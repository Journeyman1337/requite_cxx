// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <type.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::generate_logical_and_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::AND_AND);
    assert(operation.branches.size() >= 2UZ);
    this->resolver.check_type_assignable_to_type(r::BOOL_TYPE, expected_type);
    std::size_t branch_i = 0UZ;
    const r::Expression& first_expression = operation.branches.at(branch_i++);
    llvm::Value* lhs = 
        this->generate_value_expression(
            first_expression,
            r::BOOL_TYPE
        );
    while (branch_i < operation.branches.size())
    {
        const r::Expression& next_expression = operation.branches.at(branch_i++);
        llvm::Value* rhs =
            this->generate_value_expression(
                next_expression,
                r::BOOL_TYPE
            );
        lhs =
            this->llvm_builder->
                CreateLogicalAnd(
                    lhs,
                    rhs
                );            
    }
    return lhs;
}

llvm::Value* Builder::generate_logical_or_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::PIPE_PIPE);
    assert(operation.branches.size() >= 2UZ);
    std::size_t branch_i = 0UZ;
    const r::Expression& first_expression = operation.branches.at(branch_i++);
    llvm::Value* lhs = 
        this->generate_value_expression(
            first_expression,
            r::BOOL_TYPE
        );
    while (branch_i < operation.branches.size())
    {
        const r::Expression& next_expression = operation.branches.at(branch_i++);
        llvm::Value* rhs =
            this->generate_value_expression(
                next_expression,
                r::BOOL_TYPE
            );
        lhs =
            this->llvm_builder->
                CreateLogicalOr(
                    lhs,
                    rhs
                );            
    }
    return lhs;
}

llvm::Value* Builder::generate_logical_not_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::BANG);
    assert(operation.branches.size() == 1UZ);
    const r::Expression& branch_expresseion = operation.branches.front();
    llvm::Value* llvm_value = 
        this->generate_value_expression(
            branch_expresseion,
            r::BOOL_TYPE
        );
    return
        this->llvm_builder->
            CreateNot(
                llvm_value
            ); 
}

}