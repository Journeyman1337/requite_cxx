// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <procedure.hpp>
#include <procedure_group.hpp>
#include <operation.hpp>
#include <expression.hpp>
#include <type.hpp>
#include <module/module.hpp>
#include <utility.hpp>
#include <local.hpp>

#include <stdexcept>
#include <string_view>
#include <ranges>
#include <cstddef>
#include <variant>
#include <utility>
#include <format>

namespace r {

void Builder::generate_prototype(r::Procedure& procedure)
{
    assert(procedure.llvm_type == nullptr);
    assert(procedure.llvm_function == nullptr);

    this->resolver.enter(procedure);

    llvm::Type* llvm_return_type = nullptr;
    if (procedure.get_has_sret())
    {
        llvm_return_type = this->resolver.get_llvm_type(r::SpecialType::VOID);
    }
    else
    {
        llvm_return_type = this->resolver.get_llvm_type(procedure.return_type);
    }

    if (
        procedure.arguments.empty() &&
        !procedure.get_is_instanced() &&
        !procedure.get_has_sret()
    )
    {
        procedure.llvm_type =
            llvm::FunctionType::get(
                llvm_return_type,
                procedure.has_variadic_arguments
            );  
    }
    else
    {
        llvm::SmallVector<llvm::Type*> llvm_argument_types{};
        std::size_t argument_count = procedure.arguments.size();
        if (procedure.get_has_sret())
        {
            argument_count++;
        }
        if (procedure.get_is_instanced() && !procedure.get_is_constructor())
        {
            argument_count++;
        }
        llvm_argument_types.reserve(argument_count);
        if (procedure.get_has_sret())
        {
            assert(procedure.return_type.get_is_llvm_store_type());
            r::Type sret_type = procedure.get_sret_type();
            llvm::Type* llvm_sret_type =
                this->resolver.get_llvm_type(
                    sret_type
                );
            llvm_argument_types.push_back(llvm_sret_type);
        }
        if (procedure.get_is_instanced() && !procedure.get_is_constructor())
        {
            assert(procedure.object != nullptr);
            r::Type instance_object_type = r::Type(procedure.object);
            instance_object_type.add_pointer();
            llvm::Type* llvm_this_type =
                this->resolver.get_llvm_type(
                    instance_object_type
                );
            llvm_argument_types.push_back(llvm_this_type);
        }
        for (const r::ProcedureArgument& argument : procedure.arguments)
        {
            llvm_argument_types.push_back(
                this->resolver.get_llvm_type(
                    argument.type
                )
            );
        }
        procedure.llvm_type = 
            llvm::FunctionType::get(
                llvm_return_type,
                llvm_argument_types,
                procedure.has_variadic_arguments
            );
    }

    const llvm::Function::LinkageTypes llvm_linkage =
        (
            (procedure.category == r::ProcedureCategory::ENTRY_POINT) ||
            (procedure.category == r::ProcedureCategory::EXTERNAL_FUNCTION) ||
            (procedure.export_group != nullptr)
        ) ?
        llvm::Function::ExternalLinkage :
        llvm::Function::InternalLinkage;

    procedure.llvm_function =
        llvm::Function::Create(
            procedure.llvm_type,
            llvm_linkage,
            procedure.mangled_name,
            procedure.module->llvm_module.get()
        );

    llvm::Argument* llvm_arg = procedure.llvm_function->arg_begin();

    if (procedure.get_has_sret())
    {
        llvm_arg->
            addAttr(
                llvm::Attribute::get(
                    this->resolver.get_llvm_context(),
                    llvm::Attribute::StructRet
                )
            );
        if (procedure.get_is_constructor())
        {
            llvm_arg->setName("_____sret_this");
        }
        else
        {
            llvm_arg->setName("_____sret");
        }
        llvm_arg++;
    }

    if (procedure.get_is_instanced() && !procedure.get_is_constructor())
    {
        llvm_arg->setName("_____this");
        llvm_arg++;
    }

    for (r::ProcedureArgument& argument : procedure.arguments)
    {
        llvm_arg->setName(std::format("_____arg_{}", argument.name));
        llvm_arg++;
    }

    llvm::CallingConv::ID llvm_call_conv = r::to_llvm_call_conv(procedure.calling_convention);
    procedure.llvm_function->setCallingConv(llvm_call_conv);

    this->resolver.clear();
}

void Builder::generate_body(r::Procedure& procedure)
{
    if (procedure.category == r::ProcedureCategory::EXTERNAL_FUNCTION)
    {
        return;
    }

    assert(this->get_is_initialized());
    assert(procedure.llvm_type != nullptr);
    assert(procedure.llvm_function != nullptr);
    assert(this->current_block == nullptr);
    assert(procedure.module != nullptr);
    assert(procedure.module->binary != nullptr);
    assert(procedure.module->llvm_module != nullptr);
    assert(this->scopes.empty());
    assert(this->local_table.empty());
    assert(this->locals.empty());

    this->resolver.enter(procedure);

    this->push_scope();

    llvm::BasicBlock* entry_block = 
        this->create_block(
            "entry_block"
        );
    this->set_current_block(entry_block);

    llvm::BasicBlock* start_block = 
        this->create_block(
            "start_block"
        );
    this->llvm_builder->CreateBr(start_block);
    this->set_current_block(start_block);

    if (procedure.get_is_constructor())
    {
        this->generate_property_initializers(procedure);
    }

    if (procedure.declaration == nullptr)
    {
        assert(procedure.get_is_default());
        assert(procedure.return_type.get_is_void());
        this->pop_scope();
        this->llvm_builder->CreateRetVoid();
        assert(this->scopes.empty());
        this->current_block = nullptr;
        this->llvm_builder->ClearInsertionPoint();
        return;
    }

    this->generate_arguments();

    r::BreakType break_type = r::BreakType::NONE;
    for (
        const r::Expression& expression :
        procedure.declaration->branches |
        std::views::drop(procedure.body_start_i)
    )
    {
        assert(std::holds_alternative<r::Operation>(expression));
        const r::Operation& operation = std::get<r::Operation>(expression);
        break_type = this->generate_statement(operation);
        if (break_type != r::BreakType::NONE)
        {
            break;
        }
    }

    if (break_type == r::BreakType::NONE)
    {
        this->generate_appended_return(procedure);
    }

    this->pop_scope(); 

    assert(this->scopes.empty());
    this->finish_frame();
    this->current_block = nullptr;
    this->llvm_builder->ClearInsertionPoint();
}

void Builder::generate_appended_return(r::Procedure& procedure)
{
    assert(procedure.declaration != nullptr);
    llvm::BasicBlock* return_block =
    this->create_block(
        "appended_return"
    );
    this->llvm_builder->CreateBr(return_block);
    this->set_current_block(return_block);
    if (procedure.category == r::ProcedureCategory::ENTRY_POINT)
    {
        this->llvm_builder->CreateRet(
            this->llvm_builder->getInt32(0)
        );
    }
    else if (procedure.return_type.get_is_void())
    {
        this->llvm_builder->CreateRetVoid();
    }
    else
    {
        r::unreachable();
    }
}

}