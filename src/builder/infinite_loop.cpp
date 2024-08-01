// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>

#include <cassert>

namespace r {

void Builder::generate_infinite_loop_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::INFINITE_LOOP);
    if (operation.branches.empty())
    {
        return;
    }
    this->push_scope();
    llvm::BasicBlock* llvm_loop_block = this->create_block("infinite_loop");
    llvm::BasicBlock* llvm_merge_block = this->create_block("infinite_loop_merge");
    this->llvm_continue_stack.push_back(llvm_loop_block);
    this->llvm_break_stack.push_back(llvm_merge_block);
    this->llvm_builder->CreateBr(llvm_loop_block);
    this->set_current_block(llvm_loop_block);
    r::BreakType break_type = r::BreakType::NONE;
    for (const r::Expression& loop_expression : operation.branches)
    {
        assert(std::holds_alternative<r::Operation>(loop_expression));
        const r::Operation& loop_operation = std::get<r::Operation>(loop_expression);
        break_type =
            this->generate_statement(
                loop_operation
            );
        if (break_type != r::BreakType::NONE)
        {
            break;
        }
    }
    if (break_type == r::BreakType::NONE)
    {
        this->llvm_builder->CreateBr(llvm_loop_block);
    };
    this->set_current_block(llvm_merge_block);
    this->pop_scope();
    this->llvm_continue_stack.pop_back();
    this->llvm_break_stack.pop_back();
}

}