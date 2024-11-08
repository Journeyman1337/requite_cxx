// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <expression.hpp>
#include <type.hpp>
#include <utility.hpp>

#include <llvm/IR/Value.h>

#include <cassert>
#include <span>
#include <cstddef>

namespace r {

llvm::Value* Builder::generate_add_group_expression(std::span<const r::Expression> span, const r::Type& expected_type)
{
    assert(span.size() >= 2UZ);
    r::Type group_type = this->resolver.deduce_group_type(span, this);
    this->resolver.check_type_assignable_to_type(group_type, expected_type);
    group_type.clear_literals();
    if (group_type.get_is_integer())
    {
        std::size_t branch_i = 0UZ;
        const r::Expression& first_expression = span[branch_i++];
        llvm::Value* lhs = 
            this->generate_value_expression(
                first_expression,
                group_type
            );
        while (branch_i < span.size())
        {
            const r::Expression& next_expression = span[branch_i++];
            llvm::Value* rhs =
                this->generate_value_expression(
                    next_expression,
                    group_type
                );
            lhs =
                this->llvm_builder->
                    CreateAdd(
                        lhs,
                        rhs
                    );            
        }
        return lhs;
    }
    else if (group_type.get_is_floating_point())
    {
        std::size_t branch_i = 0UZ;
        const r::Expression& first_expression = span[branch_i++];
        llvm::Value* lhs = 
            this->generate_value_expression(
                first_expression,
                group_type
            );
        while (branch_i < span.size())
        {
            const r::Expression& next_expression = span[branch_i++];
            llvm::Value* rhs =
                this->generate_value_expression(
                    next_expression,
                    group_type
                );
            lhs =
                this->llvm_builder->
                    CreateFAdd(
                        lhs,
                        rhs
                    );            
        }
        return lhs;
    }
    else
    {
        throw std::runtime_error("must be numeric primitive type.");
    }
    r::unreachable();
}

llvm::Value* Builder::generate_add_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::PLUS);
    assert(operation.branches.size() >= 2UZ);
    return this->generate_add_group_expression(operation.branches, expected_type);
}

void Builder::generate_add_assignment_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::PLUS_EQUAL);
    assert(operation.branches.size() >= 2UZ);
    const r::Expression& store_location_expression = operation.branches.front();
    r::Type store_type = this->resolver.deduce_type(store_location_expression, this);
    llvm::Value* llvm_value =
        this->generate_add_group_expression(
            operation.branches,
            store_type
        );
    llvm::Value* llvm_store_location =
        this->generate_store_location(
            store_location_expression,
            true
        );
    this->generate_value_assignment(
        llvm_store_location,
        llvm_value
    );
}

llvm::Value* Builder::generate_add_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::PLUS_EQUAL);
    const r::Expression& store_location_expression = operation.branches.front();
    r::Type store_type = this->resolver.deduce_type(store_location_expression, this);
    this->resolver.check_type_assignable_to_type(store_type, expected_type);
    llvm::Value* llvm_value =
        this->generate_add_group_expression(
            operation.branches,
            store_type
        );
    llvm::Value* llvm_store_location =
        this->generate_store_location(
            store_location_expression,
            true
        );
    this->generate_value_assignment(
        llvm_store_location,
        llvm_value
    );
    return llvm_value;
}

}