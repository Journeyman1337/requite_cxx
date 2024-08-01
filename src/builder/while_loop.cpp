// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>

#include <cassert>
#include <ranges>

namespace r {

void Builder::generate_while_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::WHILE);
    assert(operation.branches.size() >= 1UZ);
    const r::Expression& while_expression = operation.branches.front();
    llvm::BasicBlock* llvm_continue_block = this->create_block("while_continue");
    llvm::BasicBlock* llvm_body_block = this->create_block("while_body");
    llvm::BasicBlock* llvm_merge_block = this->create_block("while_merge");
    this->llvm_continue_stack.push_back(llvm_continue_block);
    this->llvm_break_stack.push_back(llvm_merge_block);
    this->llvm_builder->CreateBr(llvm_continue_block);
    this->set_current_block(llvm_continue_block);
    this->push_scope();
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
    this->llvm_continue_stack.push_back(llvm_continue_block);
    this->llvm_break_stack.push_back(llvm_merge_block);
    this->set_current_block(llvm_body_block);
    this->push_scope();
    r::BreakType break_type = r::BreakType::NONE;
    for (const r::Expression& while_expression : operation.branches | std::views::drop(1UZ))
    {
        assert(std::holds_alternative<r::Operation>(while_expression));
        const r::Operation& while_operation = std::get<r::Operation>(while_expression);
        break_type =
            this->generate_statement(
                while_operation
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
    this->set_current_block(llvm_merge_block);
    this->pop_scope();
    this->llvm_continue_stack.pop_back();
    this->llvm_break_stack.pop_back();
}

}