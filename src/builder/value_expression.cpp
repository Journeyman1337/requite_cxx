// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::attribute_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::ATTRIBUTES);
    return nullptr;
}

llvm::Value* Builder::generate_value_expression(const r::Expression& expression, const r::Type& expected_type)
{
    if (std::holds_alternative<std::string_view>(expression))
    {
        std::string_view name = std::get<std::string_view>(expression);
        r::Local& local = this->get_local(name);
        this->resolver.check_type_assignable_to_type(local.type, expected_type);
        return
            this->llvm_builder->
                CreateLoad(
                    local.llvm_alloca->getAllocatedType(),
                    local.llvm_alloca,
                    name
                );
    }
    else if (std::holds_alternative<r::Literal>(expression))
    {
        const r::Literal literal = std::get<r::Literal>(expression);
        if (expected_type.get_is_numeric_primitive())
        {
            return
                this->generate_primitive_literal(
                    literal,
                    false,
                    expected_type                
                );
        }
        else if (expected_type.get_is_pointer_to_codeunit())
        {
            return
                this->generate_global_string(
                    literal
                );
        }
        r::unreachable();
    }
    else if (std::holds_alternative<r::Operation>(expression))
    {
        const r::Operation& operation = std::get<r::Operation>(expression);
        switch (operation.opcode)
        {
            case r::Opcode::LOCAL:
                return this->generate_local_value_expression(operation, nullptr, expected_type);
            case r::Opcode::MINUS:
                if (operation.branches.size() == 1UZ)
                {
                    return this->generate_negative_value_expression(operation, expected_type);
                }
                return this->generate_sub_value_expression(operation, expected_type);
            case r::Opcode::PLUS:
                if (operation.branches.size() == 1UZ)
                {
                    return this->generate_positive_value_expression(operation, expected_type);
                }
                return this->generate_add_value_expression(operation, expected_type);
            case r::Opcode::STAR:
                return this->generate_mul_value_expression(operation, expected_type);
            case r::Opcode::DIVIDE:
                return this->generate_div_value_expression(operation, expected_type);
            case r::Opcode::MODULUS:
                return this->generate_rem_value_expression(operation, expected_type);
            case r::Opcode::AND_AND:
                return this->generate_logical_and_value_expression(operation, expected_type);
            case r::Opcode::PIPE_PIPE:
                return this->generate_logical_or_value_expression(operation, expected_type);
            case r::Opcode::BANG:
                return this->generate_logical_not_value_expression(operation, expected_type);
            case r::Opcode::TRUE:
                return this->generate_true_constant_expression(operation, expected_type);
            case r::Opcode::FALSE:
                return this->generate_false_constant_expression(operation, expected_type);
            case r::Opcode::GREATER:
                return this->generate_compare_gt_value_expression(operation, expected_type);
            case r::Opcode::GREATER_EQUAL:
                return this->generate_compare_gteq_value_expression(operation, expected_type);
            case r::Opcode::LESS:
                return this->generate_compare_lt_value_expression(operation, expected_type);
            case r::Opcode::LESS_EQUAL:
                return this->generate_compare_lteq_value_expression(operation, expected_type);
            case r::Opcode::EQUAL_EQUAL:
                return this->generate_compare_eq_value_expression(operation, expected_type);
            case r::Opcode::BANG_EQUAL:
                return this->generate_compare_neq_value_expression(operation, expected_type);
            case r::Opcode::AND:
                return this->generate_bitwise_and_value_expression(operation, expected_type);
            case r::Opcode::AND_EQUAL:
                return this->generate_bitwise_and_assignment_value_expression(operation, expected_type);
            case r::Opcode::PIPE:
                return this->generate_bitwise_or_value_expression(operation, expected_type);
            case r::Opcode::PIPE_EQUAL:
                return this->generate_bitwise_or_assignment_value_expression(operation, expected_type);
            case r::Opcode::CAROT:
                return this->generate_bitwise_xor_value_expression(operation, expected_type);
            case r::Opcode::CAROT_EQUAL:
                return this->generate_bitwise_xor_assignment_value_expression(operation, expected_type);
            case r::Opcode::TILDE:
                return this->generate_bitwise_complement_value_expression(operation, expected_type);
            case r::Opcode::EQUAL:
                return this->generate_assignment_value_expression(operation, expected_type);
            case r::Opcode::QUESTION:
                return this->generate_ternary_value_expression(operation, expected_type);
            case r::Opcode::CALL:
                return this->generate_call_value_expression(operation, expected_type);
            case r::Opcode::CONSTRUCT:
                return this->generate_construct_value_expression(operation, expected_type);
            case r::Opcode::BIT_CAST:
                return this->generate_bit_cast_value_expression(operation, expected_type);
            case r::Opcode::ACCESS_TABLE:
                return this->generate_access_table_value_expression(operation, expected_type);
            case r::Opcode::ACCESS_MEMBER:
                return this->generate_access_member_value_expression(operation, expected_type);
            case r::Opcode::DEREFERENCE:
                return this->generate_dereference_value_expression(operation, expected_type);
            case r::Opcode::INDEX_INTO:
                return this->generate_index_into_value_expression(operation, expected_type);
            case r::Opcode::ADDRESS_OF:
                return this->generate_address_of_value_expression(operation, expected_type);
            case r::Opcode::ACCESS_VARIADIC_ARGUMENT:
                return this->generate_access_variadic_argument_value_expression(operation, expected_type);
            default:
                break;
        }
    }
    r::unreachable();
}

}