// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <type.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::generate_compare_eq_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::EQUAL_EQUAL);
    assert(operation.branches.size() >= 2UZ);
    this->resolver.check_type_assignable_to_type(r::BOOL_TYPE, expected_type);
    r::Type argument_type = this->resolver.deduce_group_type(operation.branches, this);
    if (argument_type.get_is_integer() || argument_type.get_is_bool())
    {
        std::size_t branch_i = 0UZ;
        const r::Expression& first_expression = operation.branches.at(branch_i++);
        llvm::Value* lhs = 
            this->generate_value_expression(
                first_expression,
                argument_type
            );
        llvm::Value* final_result = nullptr;
        while (branch_i < operation.branches.size())
        {
            const r::Expression& next_expression = operation.branches.at(branch_i++);
            llvm::Value* rhs =
                this->generate_value_expression(
                    next_expression,
                    argument_type
                );
            llvm::Value* result = 
                this->llvm_builder->
                    CreateICmpEQ(
                        lhs,
                        rhs
                    );
            if (final_result == nullptr)
            {
                final_result = result;
            }
            else
            {
                final_result =
                    this->llvm_builder->
                        CreateLogicalAnd(
                            final_result,
                            result
                        );
            }
            lhs = rhs;            
        }
        return final_result;
    }
    else if (argument_type.get_is_floating_point())
    {
        std::size_t branch_i = 0UZ;
        const r::Expression& first_expression = operation.branches.at(branch_i++);
        llvm::Value* lhs = 
            this->generate_value_expression(
                first_expression,
                argument_type
            );
        llvm::Value* final_result = nullptr;
        while (branch_i < operation.branches.size())
        {
            const r::Expression& next_expression = operation.branches.at(branch_i++);
            llvm::Value* rhs =
                this->generate_value_expression(
                    next_expression,
                    argument_type
                );
            llvm::Value* result = 
                this->llvm_builder->
                    CreateFCmpOEQ(
                        lhs,
                        rhs
                    );
            if (final_result == nullptr)
            {
                final_result = result;
            }
            else
            {
                final_result =
                    this->llvm_builder->
                        CreateLogicalAnd(
                            final_result,
                            result
                        );
            }
            lhs = rhs;            
        }
        return final_result;
    }
    else 
    {
        throw std::runtime_error("invalid type.");
    }
    r::unreachable();
}

llvm::Value* Builder::generate_compare_neq_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::BANG_EQUAL);
    assert(operation.branches.size() >= 2UZ);
    this->resolver.check_type_assignable_to_type(r::BOOL_TYPE, expected_type);
    r::Type argument_type = this->resolver.deduce_group_type(operation.branches, this);
    if (argument_type.get_is_integer() || argument_type.get_is_bool())
    {
        std::size_t branch_i = 0UZ;
        const r::Expression& first_expression = operation.branches.at(branch_i++);
        llvm::Value* lhs = 
            this->generate_value_expression(
                first_expression,
                argument_type
            );
        llvm::Value* final_result = nullptr;
        while (branch_i < operation.branches.size())
        {
            const r::Expression& next_expression = operation.branches.at(branch_i++);
            llvm::Value* rhs =
                this->generate_value_expression(
                    next_expression,
                    argument_type
                );
            llvm::Value* result = 
                this->llvm_builder->
                    CreateICmpNE(
                        lhs,
                        rhs
                    );
            if (final_result == nullptr)
            {
                final_result = result;
            }
            else
            {
                final_result =
                    this->llvm_builder->
                        CreateLogicalAnd(
                            final_result,
                            result
                        );
            }
            lhs = rhs;            
        }
        return final_result;
    }
    else if (argument_type.get_is_floating_point())
    {
        std::size_t branch_i = 0UZ;
        const r::Expression& first_expression = operation.branches.at(branch_i++);
        llvm::Value* lhs = 
            this->generate_value_expression(
                first_expression,
                argument_type
            );
        llvm::Value* final_result = nullptr;
        while (branch_i < operation.branches.size())
        {
            const r::Expression& next_expression = operation.branches.at(branch_i++);
            llvm::Value* rhs =
                this->generate_value_expression(
                    next_expression,
                    argument_type
                );
            llvm::Value* result = 
                this->llvm_builder->
                    CreateFCmpONE(
                        lhs,
                        rhs
                    );
            if (final_result == nullptr)
            {
                final_result = result;
            }
            else
            {
                final_result =
                    this->llvm_builder->
                        CreateLogicalAnd(
                            final_result,
                            result
                        );
            }
            lhs = rhs;            
        }
        return final_result;
    }
    else 
    {
        throw std::runtime_error("invalid type.");
    }
    r::unreachable();
}

}