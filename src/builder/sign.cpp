// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::generate_negative_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::MINUS);
    assert(operation.branches.size() == 1UZ);
    if (!expected_type.get_is_numeric_primitive())
    {
        throw std::runtime_error("invalid type.");
    }
    const r::Expression& branch_expression = operation.branches.at(0UZ);
    r::Type value_type = this->resolver.deduce_type(branch_expression, this);
    this->resolver.check_type_assignable_to_type(value_type, expected_type);
    if (std::holds_alternative<r::Literal>(branch_expression))
    {
        const r::Literal& branch_literal = std::get<r::Literal>(branch_expression);
        assert(branch_literal.type != r::LiteralType::CODEUNIT);
        assert(branch_literal.type != r::LiteralType::STRING);
        return this->generate_primitive_literal(branch_literal, true, expected_type);
    }
    else
    {
        llvm::Value* llvm_value =
            this->generate_value_expression(
                branch_expression,
                expected_type
            );
        if (expected_type.get_is_integer())
        {
            return
                this->llvm_builder->
                    CreateNeg(
                        llvm_value
                    );
        }
        else if (expected_type.get_is_floating_point())
        {
            return
                this->llvm_builder->
                    CreateFNeg(
                        llvm_value
                    );
        }
    }
    r::unreachable();
}

llvm::Value* Builder::generate_positive_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::PLUS);
    assert(operation.branches.size() == 1UZ);
    const r::Expression& value_expression = operation.branches.front();
    r::Type value_type = this->resolver.deduce_type(value_expression, this);
    this->resolver.check_type_assignable_to_type(value_type, expected_type);
    return 
        this->generate_value_expression(
            value_expression,
            expected_type
        );
}

}