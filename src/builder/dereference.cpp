// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <binary.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::generate_dereference_location(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::DEREFERENCE);
    assert(operation.branches.size() == 1UZ);  
    const r::Expression& variable_expression = operation.branches.front();
    r::Type type = this->resolver.deduce_type(variable_expression, this);
    assert(type.get_is_pointer());
    llvm::Type* llvm_type = this->resolver.get_llvm_type(type);
    llvm::Value* llvm_value =
        this->generate_store_location(
            variable_expression,
            false
        );
    llvm::Value* llvm_dereferenced_value =
        this->llvm_builder->
            CreateLoad(
                llvm_type,
                llvm_value,
                "dereference"
            );
    return llvm_dereferenced_value;
}

llvm::Value* Builder::generate_dereference_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::DEREFERENCE);
    assert(operation.branches.size() == 1UZ);  
    const r::Expression& branch_expression = operation.branches.front();
    r::Type deduced_type = this->resolver.deduce_type(branch_expression, this);
    this->resolver.check_dereferenced_type_assignable_to_type(deduced_type, expected_type);
    llvm::Value* llvm_dereference_location = this->generate_dereference_location(operation);
    llvm::Type* llvm_type = this->resolver.get_llvm_type(expected_type);
    return         
        this->llvm_builder->
            CreateLoad(
                llvm_type,
                llvm_dereference_location,
                "dereferenced"
            );
}

}