// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <break_type.hpp>
#include <operation.hpp>
#include <opcode.hpp>

#include <cassert>
#include <ranges>

namespace r {

void Builder::generate_switch_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::SWITCH);
    assert(!operation.branches.empty());
    const r::Expression& switch_argument_expression = operation.branches.front();
    r::Type type = this->resolver.deduce_type(switch_argument_expression, this);
    type.clear_literals();
    llvm::BasicBlock* llvm_switch_block = this->create_block("switch");
    this->llvm_builder->CreateBr(llvm_switch_block);
    this->set_current_block(llvm_switch_block);
    llvm::BasicBlock* llvm_merge_block = this->create_block("switch_merge");
    this->llvm_break_stack.push_back(llvm_merge_block);
    this->push_scope();
    llvm::Value* llvm_switch_value = 
        this->generate_value_expression(
            switch_argument_expression,
            type
        );
    llvm::BasicBlock* llvm_default_case_block = nullptr;
    if (operation.branches.size() > 1UZ)
    {
        const r::Expression& last_expression = operation.branches.back();
        assert(std::holds_alternative<r::Operation>(last_expression));
        const r::Operation& last_operation = std::get<r::Operation>(last_expression);
        if (last_operation.opcode == r::Opcode::DEFAULT)
        {
            llvm_default_case_block = this->create_block("default_case");
        }
    }
    if (llvm_default_case_block == nullptr)
    {
        llvm_default_case_block = llvm_merge_block;
    }    
    llvm::SwitchInst* llvm_switch =
        this->llvm_builder->
            CreateSwitch(
                llvm_switch_value,
                llvm_default_case_block,
                operation.branches.size() - 1UZ
            );
    r::BreakType break_type = r::BreakType::NONE;
    for (const r::Expression& switch_expression : operation.branches | std::views::drop(1UZ))
    {
        assert(std::holds_alternative<r::Operation>(switch_expression));
        const r::Operation& switch_operation = std::get<r::Operation>(switch_expression);
        // all expressions must be case opcode besides the last one.
        assert(
            &operation.branches.back() == &switch_expression ||
            switch_operation.opcode == r::Opcode::CASE
        );
        // the last expression may be either default or case opcode.
        assert(
            &operation.branches.back() != &switch_expression || 
            switch_operation.opcode == r::Opcode::CASE || 
            switch_operation.opcode == r::Opcode::DEFAULT
        );
        if (switch_operation.opcode == r::Opcode::CASE)
        {
            break_type = this->generate_case_statement(switch_operation, break_type, type, llvm_switch);
            continue;
        }
        else if (switch_operation.opcode == r::Opcode::DEFAULT)
        {
            this->generate_default_case_statement(switch_operation, break_type, type, llvm_default_case_block, llvm_switch);
            break;
        }
        break_type = r::BreakType::NONE;
    }
    this->pop_scope();
    this->llvm_break_stack.pop_back();
    this->set_current_block(llvm_merge_block);
}

r::BreakType Builder::generate_case_statement(const r::Operation& operation, r::BreakType prev_break_type, const r::Type& type, llvm::SwitchInst* llvm_switch)
{
    assert(operation.opcode == r::Opcode::CASE);
    assert(!operation.branches.empty());
    llvm::BasicBlock* llvm_case_block = this->create_block("case");
    if (prev_break_type == r::BreakType::FALL_THROUGH)
    {
        this->llvm_builder->
            CreateBr(
                llvm_case_block
            );
    }
    this->push_scope();
    const r::Expression& case_value_expression = operation.branches.front();
    llvm::ConstantInt* llvm_case_value = this->generate_integer_constant(case_value_expression, type);
    this->set_current_block(llvm_case_block);
    llvm_switch->
        addCase(
            llvm_case_value,
            llvm_case_block
        );
    r::BreakType break_type = r::BreakType::NONE;
    for (const r::Expression& case_expression : operation.branches | std::views::drop(1UZ))
    {
        assert(std::holds_alternative<r::Operation>(case_expression));
        const r::Operation& case_operation = std::get<r::Operation>(case_expression);
        if (case_operation.opcode == r::Opcode::FALL_THROUGH)
        {
            assert(case_operation.branches.empty());
            return r::BreakType::FALL_THROUGH;
        }
        break_type =
            this->generate_statement(
                case_operation
            );
        if (break_type != r::BreakType::NONE)
        {
            break;
        }
    }
    this->pop_scope();
    if (break_type == r::BreakType::NONE)
    {
        assert(!this->llvm_break_stack.empty());
        this->llvm_builder->
            CreateBr(
                this->llvm_break_stack.back()
            );
    }
    return break_type;
}

void Builder::generate_default_case_statement(const r::Operation& operation, r::BreakType prev_break_type, const r::Type& type, llvm::BasicBlock* llvm_default_case_block, llvm::SwitchInst* llvm_switch)
{
    assert(operation.opcode == r::Opcode::DEFAULT);
    if (prev_break_type == r::BreakType::FALL_THROUGH)
    {
        this->llvm_builder->
            CreateBr(
                llvm_default_case_block
            );
    }
    this->set_current_block(llvm_default_case_block);
    this->push_scope();
    r::BreakType break_type = r::BreakType::NONE;
    for (const r::Expression& case_expression : operation.branches)
    {
        assert(std::holds_alternative<r::Operation>(case_expression));
        const r::Operation& case_operation = std::get<r::Operation>(case_expression);
        assert(operation.opcode != r::Opcode::FALL_THROUGH);
        break_type =
            this->generate_statement(
                case_operation
            );
        if (break_type != r::BreakType::NONE)
        {
            break;
        }
    }
    this->pop_scope();
    if (break_type == r::BreakType::NONE)
    {
        assert(!this->llvm_break_stack.empty());
        this->llvm_builder->
            CreateBr(
                this->llvm_break_stack.back()
            );
    }
    llvm_switch->setDefaultDest(
        llvm_default_case_block
    );
}    

}