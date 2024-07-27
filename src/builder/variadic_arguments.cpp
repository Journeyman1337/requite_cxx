// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <expression.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <type.hpp>
#include <module/module.hpp>
#include <binary.hpp>

#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/Instructions.h>

#include <cassert>
#include <stdexcept>


namespace r {

void Builder::generate_start_variadic_arguments_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::START_VARIADIC_ARGUMENTS);
    assert(operation.branches.size() == 1UZ);
    const r::Expression& store_expression = operation.branches.front();
    r::Type deduced_type = this->resolver.deduce_type(store_expression, this);
    this->resolver.check_type_assignable_to_type(deduced_type, r::VARIADIC_ARGUMENTS_TYPE);
    llvm::Function* llvm_va_start_intrinsic =
        llvm::Intrinsic::getDeclaration(
            &this->resolver.get_llvm_module(),
            llvm::Intrinsic::vastart
        );
    llvm::Value* llvm_store_location =
        this->generate_store_location(
            store_expression,
            true
        );
    this->llvm_builder->CreateCall(
        llvm_va_start_intrinsic,
        {llvm_store_location}
    );
}

void Builder::generate_access_variadic_argument_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::ACCESS_VARIADIC_ARGUMENT);
}

llvm::Value* Builder::generate_access_variadic_argument_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    // TODO look into this (from @tnorthover on LLVM Discord):
    // Mostly, targets try to make varargs functions compatible with normal ones for 
    // backwards compatibility with dodgy software, which means va_arg needs to understand 
    // something close to the full weight of how arguments are passed to functions. That's 
    // not something we've solved in LLVM yet, and certainly va_arg's single type parameter 
    // isn't enough.
    // So the same reason front-ends get involved in lowering arguments generally I suppose.
    // https://discordapp.com/channels/636084430946959380/642374147640524831/1261752983889121331

    assert(operation.opcode == r::Opcode::ACCESS_VARIADIC_ARGUMENT);
    assert(operation.branches.size() == 2UZ);
    const r::Expression& type_expression = operation.branches.front();
    r::Type type = this->resolver.resolve_type(type_expression);
    this->resolver.check_type_assignable_to_type(type, expected_type);
    const r::Expression& store_expression = operation.branches.back();
    r::Type deduced_type = this->resolver.deduce_type(store_expression, this);
    this->resolver.check_type_assignable_to_type(deduced_type, r::VARIADIC_ARGUMENTS_TYPE);
    llvm::Value* llvm_store_value =
        this->generate_store_location(
            store_expression,
            false
        );
    llvm::Type* llvm_type =
        this->resolver.get_llvm_type(
            type
        );
    return
        this->llvm_builder->
            CreateVAArg(
                llvm_store_value,
                llvm_type,
                "access_va_arg"
            );
}

llvm::Value* Builder::generate_access_variadic_argument_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::ACCESS_VARIADIC_ARGUMENT);
    return nullptr;
}

void Builder::generate_end_variadic_arguments_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::END_VARIADIC_ARGUMENTS);
    assert(operation.branches.size() == 1UZ);
    const r::Expression& store_expression = operation.branches.front();
    r::Type deduced_type = this->resolver.deduce_type(store_expression, this);
    this->resolver.check_type_assignable_to_type(deduced_type, r::VARIADIC_ARGUMENTS_TYPE);
    llvm::Function* llvm_va_start_intrinsic =
        llvm::Intrinsic::getDeclaration(
            &this->resolver.get_llvm_module(),
            llvm::Intrinsic::vaend
        );
    llvm::Value* llvm_store_location =
        this->generate_store_location(
            store_expression,
            true
        );
    this->llvm_builder->CreateCall(
        llvm_va_start_intrinsic,
        {llvm_store_location}
    );
}

}