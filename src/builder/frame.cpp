// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <procedure.hpp>
#include <procedure_argument.hpp>
#include <local.hpp>
#include <object.hpp>
#include <binary.hpp>

#include <span>
#include <cassert>

namespace r {

void Builder::generate_arguments()
{
    auto arg_iter = this->resolver.procedure->llvm_function->arg_begin();
    if (
        this->resolver.procedure->get_is_constructor() ||
        this->resolver.procedure->get_is_destructor()
    )
    {
        arg_iter++;
    }
    else 
    {
        if (this->resolver.procedure->get_has_sret())
        {
            arg_iter++;
        }
        if (this->resolver.procedure->get_is_instanced())
        {
            arg_iter++;
        }
    }
    for (const r::ProcedureArgument& argument : this->resolver.procedure->arguments)
    {
        r::Local& local =
            this->
                add_local(
                    argument.name,
                    argument.type
                );
        this->generate_local(local);
        llvm::Value* llvm_argument_value = &(*arg_iter++);
        llvm_argument_value->setName(argument.name);
        local.has_value = true;
        this->
            generate_value_assignment(
                local.llvm_alloca,
                llvm_argument_value
            );
    }
}

void Builder::push_scope()
{
    this->scopes.emplace_back();
}

void Builder::pop_scope()
{
    assert(!this->scopes.empty());
    this->generate_autodestruct_scope();
    const llvm::SmallVector<std::size_t>& top_scope = this->scopes.back();
    for (const std::size_t& local_i : top_scope)
    {
        r::Local& local = *this->locals[local_i].get();
        assert(this->local_table.contains(local.name));
        this->local_table.erase(local.name);
    }
    this->locals.erase(this->locals.end() - top_scope.size(), this->locals.end());
    this->scopes.pop_back();
}

void Builder::finish_frame()
{
    for (const llvm::StringMapEntry<r::Label>& label : this->label_table)
    {
        if (!label.second.is_placed)
        {
            throw std::runtime_error("GO_TO label must exist.");
        }
    }
    this->label_table.clear();
    this->locals.clear();
    this->scopes.clear();
    this->local_table.clear();
}

void Builder::clear_temporaries()
{
    for (llvm::detail::DenseMapPair<const r::Operation*, r::Temporary>& temporary_pair : this->temporary_table)
    {
        this->generate_destruct(temporary_pair.second);
    }
    this->temporary_table.clear();
}


r::Local& Builder::add_local(std::string_view name, const r::Type& type)
{
    assert(!this->scopes.empty());
    if (this->local_table.contains(name))
    {
        throw std::runtime_error("duplicate local variable with name.");
    }
    if (type.get_has_null_root())
    {
        throw std::runtime_error("store types must not have null root.");
    }
    std::unique_ptr<r::Local>& local_ptr = this->locals.emplace_back();
    local_ptr = std::make_unique<r::Local>();
    r::Local& local = *local_ptr.get();
    local.name = name;
    local.type = type;
    this->local_table[name] = &local;
    llvm::SmallVector<std::size_t>& back_scope = this->scopes.back();
    back_scope.push_back(this->locals.size() - 1UZ);
    return local;
}

r::Local* Builder::try_get_local(std::string_view name)
{
    if (!this->local_table.contains(name))
    {
        return nullptr;
    }
    return this->local_table.at(name);
}

r::Local& Builder::get_local(std::string_view name)
{
    r::Local* local = this->try_get_local(name);
    if (local == nullptr)
    {
        throw std::runtime_error("local with name not found.");
    }
    return *local;
}

r::Temporary* Builder::try_get_temporary(const r::Operation* operation_ptr)
{
    if (!this->temporary_table.contains(operation_ptr))
    {
        return nullptr;
    }
    return &this->temporary_table[operation_ptr];
}

r::Temporary& Builder::add_temporary(const r::Operation* operation_ptr, const r::Type& type)
{
    assert(!this->temporary_table.contains(operation_ptr));
    r::Temporary& temporary = this->temporary_table[operation_ptr];
    temporary.type = type;
    return temporary;
}

r::Temporary& Builder::get_temporary(const r::Operation* operation_ptr)
{
    r::Temporary* temporary = this->try_get_temporary(operation_ptr);
    assert(temporary != nullptr);
    return *temporary;
}

void Builder::generate_local(r::Local& local, llvm::Value* llvm_dynamic_array_size)
{
    assert(local.llvm_alloca == nullptr);
    llvm::Type* llvm_type =
        this->resolver.get_llvm_type(
            local.type
        );
    local.llvm_alloca =
        this->generate_alloca(
            llvm_type,
            local.name,
            llvm_dynamic_array_size
        );
    local.llvm_dynamic_array_size = llvm_dynamic_array_size;
}

void Builder::generate_temporary(r::Temporary& temporary, llvm::Value* llvm_dynamic_array_size)
{
    assert(temporary.llvm_alloca == nullptr);
    llvm::Type* llvm_type =
        this->resolver.get_llvm_type(
            temporary.type
        );
    temporary.llvm_alloca =
        this->generate_alloca(
            llvm_type,
            "_____temp",
            llvm_dynamic_array_size
        );
    temporary.llvm_dynamic_array_size = llvm_dynamic_array_size;
}

llvm::AllocaInst* Builder::generate_alloca(llvm::Type* llvm_type, std::string_view name, llvm::Value* llvm_dynamic_array_size)
{
    llvm::IRBuilderBase::InsertPoint old_insertion_point = this->llvm_builder->saveAndClearIP();
    this->llvm_builder->
        SetInsertPointPastAllocas(
            this->resolver.get_llvm_function()
        );
    llvm::AllocaInst* llvm_alloca = nullptr;
    if (llvm_dynamic_array_size == nullptr)
    {
        llvm_alloca =
            this->llvm_builder->
                CreateAlloca(
                    llvm_type,
                    0,
                    name
                );
    }
    else
    {
        llvm_alloca =
            this->llvm_builder->
                CreateAlloca(
                    llvm_type,
                    0,
                    llvm_dynamic_array_size,
                    name
                );  
    }
    this->llvm_builder->restoreIP(old_insertion_point);
    return llvm_alloca;
}

}