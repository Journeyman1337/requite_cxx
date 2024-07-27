// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <break_type.hpp>

#include <cassert>
#include <ranges>

namespace r {

void Builder::generate_for_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::FOR);
    assert(operation.branches.size() >= 3UZ);
    this->push_scope();
    const r::Expression& start_expression = operation.branches.front();
    assert(std::holds_alternative<r::Operation>(start_expression));
    const r::Operation& start_operation = std::get<r::Operation>(start_expression);
    const r::Expression& while_expression = operation.branches.at(1UZ);
    const r::Expression& do_expression = operation.branches.at(2UZ);
    assert(std::holds_alternative<r::Operation>(do_expression));
    const r::Operation& do_operation = std::get<r::Operation>(do_expression);
    llvm::BasicBlock* llvm_start_block = this->create_block("for_start");
    llvm::BasicBlock* llvm_continue_block = this->create_block("for_continue");
    llvm::BasicBlock* llvm_while_block = this->create_block("for_while");
    llvm::BasicBlock* llvm_body_block = this->create_block("for_body");
    llvm::BasicBlock* llvm_merge_block = this->create_block("for_merge");
    this->llvm_builder->CreateBr(llvm_start_block);
    this->set_current_block(llvm_start_block);
    this->generate_statement(
        start_operation
    );
    this->llvm_builder->CreateBr(llvm_while_block);
    this->set_current_block(llvm_while_block);
    this->llvm_continue_stack.push_back(llvm_continue_block);
    this->llvm_break_stack.push_back(llvm_merge_block);
    llvm::Value* llvm_condition = 
        this->generate_value_expression(
            while_expression,
            r::BOOL_TYPE
        );
    this->llvm_builder->
        CreateCondBr(
            llvm_condition,
            llvm_body_block,
            llvm_merge_block
        );
    this->set_current_block(llvm_body_block);
    this->push_scope();
    r::BreakType break_type = r::BreakType::NONE;
    for (const r::Expression& for_expression : operation.branches | std::views::drop(3UZ))
    {
        assert(std::holds_alternative<r::Operation>(for_expression));
        const r::Operation& for_operation = std::get<r::Operation>(for_expression);
        break_type =
            this->generate_statement(
                for_operation
            );
        if (break_type != r::BreakType::NONE)
        {
            break;
        }
    }
    this->pop_scope();
    if (break_type == r::BreakType::NONE)
    {
        this->llvm_builder->CreateBr(llvm_continue_block);
    }
    this->set_current_block(llvm_continue_block);
    this->generate_statement(
        do_operation
    );
    this->llvm_builder->CreateBr(llvm_while_block);
    this->set_current_block(llvm_merge_block);
    this->pop_scope();
    this->llvm_continue_stack.pop_back();
    this->llvm_break_stack.pop_back();
}

}