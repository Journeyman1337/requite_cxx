// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <break_type.hpp>
#include <operation.hpp>
#include <opcode.hpp>

#include <ranges>
#include <cassert>

namespace r {

void Builder::generate_condition_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::CONDITION);
    assert(operation.branches.size() >= 1UZ);
    llvm::BasicBlock* llvm_merge_block = this->create_block("condition_merge");
    this->push_scope();
    for (std::size_t condition_expression_i = 0UZ; condition_expression_i < operation.branches.size(); condition_expression_i++)
    {
        const r::Expression& condition_expression = operation.branches.at(condition_expression_i);
        assert(std::holds_alternative<r::Operation>(condition_expression));
        const r::Operation& condition_operation = std::get<r::Operation>(condition_expression);
        // first condition must use if opcode
        assert(&condition_expression != &operation.branches.front()|| condition_operation.opcode == r::Opcode::IF);
        // middle conditions must use elif opcode
        assert(&condition_expression == &operation.branches.front() || &condition_expression == &operation.branches.back() || condition_operation.opcode == r::Opcode::ELSE_IF);
        // last condition must not use if opcode if multiple condition branches
        assert(&condition_expression == &operation.branches.back() || operation.branches.size() > 1UZ || condition_operation.opcode != r::Opcode::IF);
        if (condition_operation.opcode != r::Opcode::ELSE)
        {
            llvm::BasicBlock * llvm_if_block = 
                this->create_block(
                    "if"
                );
            llvm::BasicBlock* llvm_else_block = nullptr;
            if (&condition_expression == &operation.branches.back())
            {
                llvm_else_block = llvm_merge_block;
            }
            else
            {
                llvm_else_block = 
                    this->create_block(
                        "else"
                    );
            }
            assert(!condition_operation.branches.empty());
            const r::Expression& comparison_expression = condition_operation.branches.front();
            llvm::Value* llvm_condition =
                this->generate_value_expression(
                    comparison_expression,
                    r::BOOL_TYPE
                );
            this->llvm_builder->
                CreateCondBr(
                    llvm_condition,
                    llvm_if_block,
                    llvm_else_block
                );
            this->set_current_block(llvm_if_block);
            this->push_scope();
            r::BreakType break_type = r::BreakType::NONE;
            for (const r::Expression& scope_expression : condition_operation.branches | std::views::drop(1UZ))
            {
                assert(std::holds_alternative<r::Operation>(scope_expression));
                const r::Operation& scope_operation = std::get<r::Operation>(scope_expression);
                break_type = this->generate_statement(scope_operation);
                if (break_type != r::BreakType::NONE)
                {
                    break;
                }
            }
            if (break_type == r::BreakType::NONE)
            {
                this->llvm_builder->CreateBr(llvm_merge_block);
            }
            this->pop_scope();
            this->set_current_block(llvm_else_block);
        }
        else // if (condition_operation.opcde == r::Opcode::ELSE)
        {
            this->push_scope();
            r::BreakType break_type = r::BreakType::NONE;
            for (const r::Expression& scope_expression : condition_operation.branches)
            {
                assert(std::holds_alternative<r::Operation>(scope_expression));
                const r::Operation& scope_operation = std::get<r::Operation>(scope_expression);
                break_type = this->generate_statement(scope_operation);
                if (break_type != r::BreakType::NONE)
                {
                    break;
                }
            }
            this->pop_scope();
            if (break_type == r::BreakType::NONE)
            {
                this->llvm_builder->CreateBr(llvm_merge_block);
            }
        }        
    }
    this->pop_scope();
    this->set_current_block(llvm_merge_block);
}

}