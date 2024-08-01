// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <utility.hpp>
#include <break_type.hpp>

#include <cassert>

namespace r {

r::BreakType Builder::attribute_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::ATTRIBUTES);
    assert(operation.branches.size() >= 2UZ);
    const r::Expression& last_expression = operation.branches.back();
    assert(std::holds_alternative<r::Operation>(last_expression));
    const r::Operation& last_operation = std::get<r::Operation>(last_expression);
    switch (last_operation.opcode)
    {
        case r::Opcode::LOCAL:
            this->generate_local_statement(last_operation, &operation);
            break;
        default:
            r::unreachable();
    }
    return r::BreakType::NONE;
}

r::BreakType Builder::generate_statement(const r::Operation& operation)
{
    assert(this->temporary_table.empty());
    switch (operation.opcode)
    {
        case r::Opcode::ACCESS_TABLE:
            this->generate_access_table_statement(operation);
            break;
        case r::Opcode::ASSERT:
            this->generate_assert_statement(operation);
            break;
        case r::Opcode::ASSUME:
            this->generate_assume_statement(operation);
            break;
        case r::Opcode::ATTRIBUTES:
            this->attribute_statement(operation);
            break;
        case r::Opcode::BREAK:
            this->generate_break_statement(operation);
            return r::BreakType::BREAK;
        case r::Opcode::CALL:
            this->generate_call_statement(operation);
            break;
        case r::Opcode::CAROT_EQUAL:
            this->generate_bitwise_xor_assignment_statement(operation);
            break;
        case r::Opcode::CONDITION:
            this->generate_condition_statement(operation);
            break;
        case r::Opcode::CONSTRUCT:
            this->generate_construct_statement(operation);
            break;
        case r::Opcode::CONTINUE:
            this->generate_continue_statement(operation);
            return r::BreakType::CONTINUE;
        case r::Opcode::DIVIDE_EQUAL:
            this->generate_div_assignment_statement(operation);
            break;
        case r::Opcode::DESTRUCT:
            this->generate_desruct_statement(operation);
            break;
        case r::Opcode::END_VARIADIC_ARGUMENTS:
            this->generate_end_variadic_arguments_statement(operation);
            break;
        case r::Opcode::EMPTY:
            break;
        case r::Opcode::EQUAL:
            this->generate_assignment_statement(operation);
            break;
        case r::Opcode::FOR:
            this->generate_for_statement(operation);
            break;
        case r::Opcode::GO_TO:
            this->generate_GO_TO_statement(operation);
            return r::BreakType::GO_TO;
        case r::Opcode::INFINITE_LOOP:
            this->generate_infinite_loop_statement(operation);
            break;
        case r::Opcode::LABEL:
            this->generate_label_statement(operation);
            break;
        case r::Opcode::LOCAL:
            this->generate_local_statement(operation);
            break;
        case r::Opcode::MINUS_EQUAL:
            this->generate_sub_assignment_statement(operation);
            break;
        case r::Opcode::PIPE_EQUAL:
            this->generate_bitwise_or_assignment_statement(operation);
            break;
        case r::Opcode::PLUS_EQUAL:
            this->generate_add_assignment_statement(operation);
            break;
        case r::Opcode::RETURN:
            this->generate_return_statement(operation);
            return r::BreakType::RETURN;
        case r::Opcode::QUESTION:
            this->generate_ternary_statement(operation);
            break;
        case r::Opcode::STAR_EQUAL:
            this->generate_mul_assignment_statement(operation);
            break;
        case r::Opcode::START_VARIADIC_ARGUMENTS:
            this->generate_start_variadic_arguments_statement(operation);
            break;
        case r::Opcode::SWITCH:
            this->generate_switch_statement(operation);
            break;
        case r::Opcode::UNREACHABLE:
            this->generate_unreachable_statement(operation);
            break;
        case r::Opcode::WHILE:
            this->generate_while_statement(operation);
            break;
        default:
            r::unreachable();        
    }
    this->clear_temporaries();
    return r::BreakType::NONE;
}

}