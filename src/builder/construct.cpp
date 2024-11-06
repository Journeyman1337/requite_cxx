// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <type.hpp>
#include <utility.hpp>
#include <procedure.hpp>

#include <cassert>

namespace r {

void Builder::generate_construct_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::CONSTRUCT);
    const r::Expression& type_expression = operation.branches.front();
    r::Type type = this->resolver.resolve_type(type_expression);
    type.resolve_type_alias();
    if (type.get_is_primitive())
    {
        return; // do nothing.
    }
    assert(type.get_is_object());
    r::Temporary& temporary = this->add_temporary(&operation, type);
    this->generate_temporary(temporary);
    this->generate_construct_store_expression(
        operation,
        temporary.llvm_alloca,
        type
    );
}

llvm::Value* Builder::generate_construct_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::CONSTRUCT);
    assert(!operation.branches.empty());
    assert(expected_type.get_is_llvm_value_type());
    const r::Expression& type_expression = operation.branches.front();
    r::Type type = this->resolver.resolve_type(type_expression);
    type.resolve_type_alias();
    this->resolver.check_type_assignable_to_type(type, expected_type);
    if (type.get_is_primitive())
    {
        assert(operation.branches.size() == 2UZ);
        const r::Expression& value_expression = operation.branches.back();
        r::Type deduced_type = this->resolver.deduce_type(value_expression, this);
        deduced_type.clear_literals();
        llvm::Value* llvm_value = 
            this->generate_value_expression(
                value_expression,
                type
            );
        if (r::get_types_are_equivalent(type, deduced_type))
        {
            return llvm_value;
        }
        llvm::Type* llvm_type = this->resolver.get_llvm_type(type);
        if (deduced_type.get_is_signed_integer())
        {
            if (type.get_is_integer())
            {
                return
                    this->llvm_builder->
                        CreateIntCast(
                            llvm_value,
                            llvm_type,
                            type.get_is_signed_integer(),
                            "int_cast"
                        );
            }
            else if (type.get_is_floating_point())
            {
                return
                    this->llvm_builder->
                        CreateSIToFP(
                            llvm_value,
                            llvm_type,
                            "int_cast"
                        );
            }
        }
        else if (deduced_type.get_is_unsigned_integer())
        {
            if (type.get_is_integer())
            {
                return
                    this->llvm_builder->
                        CreateIntCast(
                            llvm_value,
                            llvm_type,
                            type.get_is_signed_integer(),
                            "int_cast"
                        );
            }
            else if (type.get_is_floating_point())
            {
                return
                    this->llvm_builder->
                        CreateUIToFP(
                            llvm_value,
                            llvm_type,
                            "int_cast"
                        );
            } 
        }
        else if (deduced_type.get_is_floating_point())
        {
            if (type.get_is_signed_integer())
            {
                return
                    this->llvm_builder->
                        CreateFPToSI(
                            llvm_value,
                            llvm_type,                
                            "fp_cast"
                        );
            }
            else if (type.get_is_unsigned_integer())
            {
                return
                    this->llvm_builder->
                        CreateFPToUI(
                            llvm_value,
                            llvm_type,                
                            "fp_cast"
                        ); 
            }
            else if (type.get_is_floating_point())
            {
                return
                    this->llvm_builder->
                        CreateFPCast(
                            llvm_value,
                            llvm_type,                
                            "fp_cast"
                        ); 
            }
        }
    }
    else
    {
        r::unreachable();
    }
    r::unreachable();
}

llvm::Value* Builder::generate_construct_temporary_location(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::CONSTRUCT);
    assert(!operation.branches.empty());
    const r::Expression& type_expression = operation.branches.front();
    r::Type type = this->resolver.resolve_type(type_expression);
    r::Temporary& temporary = this->add_temporary(&operation, type);
    this->generate_temporary(temporary);
    this->generate_construct_store_expression(operation, temporary.llvm_alloca, type);
    return temporary.llvm_alloca;
}

void Builder::generate_construct_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::CONSTRUCT);
    assert(!operation.branches.empty());
    assert(llvm_store != nullptr);
    const r::Expression& type_expression = operation.branches.front();
    r::Type type = this->resolver.resolve_type(type_expression);
    type.resolve_type_alias();
    this->resolver.check_type_assignable_to_type(type, expected_type);
    if (type.get_is_llvm_store_type())
    {
        r::Procedure& callee =
            this->resolver.get_construct_procedure(
                operation,
                expected_type,
                this
        );
        llvm::SmallVector<llvm::Value*> llvm_arguments;
        // subtract one for the type branch. add one for the instance argument.
        llvm_arguments.resize(operation.branches.size() - 1UZ + 1UZ); 
        llvm_arguments.front() = llvm_store;
        for (std::size_t input_arg_i = 0UZ; input_arg_i < operation.branches.size() - 1UZ; input_arg_i++)
        { // TODO work with args that need to be temporarily stored.
            const r::Expression& arg_expression = operation.branches.at(1UZ + input_arg_i);
            r::Type arg_type{};
            const r::ProcedureArgument& procedure_arg = callee.arguments[input_arg_i];
            arg_type = procedure_arg.type;           
            llvm_arguments[input_arg_i + 1UZ] = // add 1 to skip the _____this
                this->generate_value_expression(
                    arg_expression,
                    arg_type
                );
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
    else
    {
        r::unreachable();
    }
}

}