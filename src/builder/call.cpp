// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <procedure.hpp>
#include <type.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {
    
void Builder::generate_call_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::CALL);
    assert(!operation.branches.empty());
    r::Procedure& callee =
        this->resolver.get_call_procedure(
            operation,
            this
        );
    const r::Expression& call_expression = operation.branches.front();
    std::vector<llvm::Value*> llvm_arguments;
    std::size_t argument_count = operation.branches.size() - 1UZ;
    if (callee.get_has_sret())
    {
        argument_count++;
    }
    if (callee.get_is_instanced())
    {
        argument_count++;
    }
    llvm_arguments.reserve(argument_count);
    if (callee.get_has_sret())
    {
        r::Type sret_type = callee.get_sret_type();
        llvm::Type* llvm_sret_type = this->resolver.get_llvm_type(sret_type);
        llvm::AllocaInst* llvm_temp = this->generate_alloca(llvm_sret_type, "temp");
        llvm_arguments.push_back(llvm_temp);
    }
    if (callee.get_is_instanced())
    {
        llvm::Value* llvm_caller =
            this->generate_owner_location(
                call_expression
            );
        llvm_arguments.push_back(llvm_caller);        
    }
    if (!callee.arguments.empty())
    {
        for (std::size_t input_arg_i = 0UZ; input_arg_i < operation.branches.size() - 1UZ; input_arg_i++)
        { // TODO work with args that need to be temporarily stored.
            const r::Expression& arg_expression = operation.branches.at(1UZ + input_arg_i);
            r::Type arg_type{};
            if (input_arg_i >= callee.arguments.size())
            { // deduce variadic argument type.
                assert(callee.has_variadic_arguments);
                arg_type = this->resolver.deduce_type(arg_expression, this);
            }
            else
            {
                const r::ProcedureArgument& procedure_arg = callee.arguments.at(input_arg_i);
                arg_type = procedure_arg.type;
            }      
            llvm::Value* llvm_argument =
                this->generate_value_expression(
                    arg_expression,
                    arg_type
                );      
            if (input_arg_i >= callee.arguments.size())
            {
                llvm_argument->setName(std::format("variadic_arg_{}", input_arg_i - callee.arguments.size()));
            }
            else
            {
                const r::ProcedureArgument& procedure_arg = callee.arguments.at(input_arg_i);
                llvm_argument->setName(procedure_arg.name);
            }     
            llvm_arguments.push_back(
                llvm_argument
            );
        }
    }
    for (const llvm::Value* llvm_arg_value : llvm_arguments)
    {
        assert(llvm_arg_value != nullptr);
    }
    this->llvm_builder->
        CreateCall(
            callee.llvm_function,
            llvm_arguments,
            "call"
        );
}

llvm::Value* Builder::generate_call_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::CALL);
    return nullptr;
}

void Builder::generate_call_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::CALL);
    assert(!operation.branches.empty());
    r::Procedure& callee =
        this->resolver.get_call_procedure(
            operation,
            this
        );
    this->resolver.check_type_assignable_to_type(callee.return_type, expected_type);
    const r::Expression& call_expression = operation.branches.front();
    std::vector<llvm::Value*> llvm_arguments;
    std::size_t argument_count = operation.branches.size() - 1UZ;
    argument_count++; // for sret
    if (callee.get_is_instanced() && !callee.get_is_constructor())
    {
        argument_count++;
    }
    llvm_arguments.reserve(argument_count);
    if (callee.get_has_sret())
    {
        llvm_arguments.push_back(llvm_store);
    }
    if (callee.get_is_instanced())
    {
        llvm::Value* llvm_caller =
            this->generate_owner_location(
                call_expression
            );
        llvm_arguments.push_back(llvm_caller);        
    }
    if (!callee.arguments.empty())
    {
        for (std::size_t input_arg_i = 0UZ; input_arg_i < operation.branches.size() - 1UZ; input_arg_i++)
        { // TODO work with args that need to be temporarily stored.
            const r::Expression& arg_expression = operation.branches.at(1UZ + input_arg_i);
            r::Type arg_type{};
            if (input_arg_i >= callee.arguments.size())
            { // deduce variadic argument type.
                assert(callee.has_variadic_arguments);
                arg_type = this->resolver.deduce_type(arg_expression, this);
            }
            else
            {
                const r::ProcedureArgument& procedure_arg = callee.arguments.at(input_arg_i);
                arg_type = procedure_arg.type;
            }      
            llvm::Value* llvm_argument =
                this->generate_value_expression(
                    arg_expression,
                    arg_type
                );
            llvm_arguments.push_back(
                llvm_argument
            );
        }
    }
    for (const llvm::Value* llvm_arg_value : llvm_arguments)
    {
        assert(llvm_arg_value != nullptr);
    }
    this->llvm_builder->
        CreateCall(
            callee.llvm_function,
            llvm_arguments,
            "call"
        );
}

}