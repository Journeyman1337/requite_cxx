// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <procedure.hpp>
#include <type.hpp>
#include <binary.hpp>
#include <utility.hpp>
#include <string_utility.hpp>

#include <cassert>

namespace r {

void Builder::generate_return_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::RETURN);
    assert(operation.branches.size() <= 1UZ);
    const r::Type& return_type = this->resolver.procedure->return_type;
    // TODO dont autodestroy anything that is returned.
    this->generate_autodestruct_frame();
    if (operation.branches.empty())
    {
        assert(return_type.get_is_void());
        this->llvm_builder->
            CreateRetVoid();
    }
    else
    {
        const r::Expression& expression = operation.branches.front();
        r::Type type = this->resolver.deduce_type(expression, this);
        type.resolve_type_alias();
        this->resolver.check_type_assignable_to_type(type, return_type);
        if (type.get_is_llvm_store_type())
        {
            // return to sret parameter.
            // https://discourse.llvm.org/t/about-the-optimization-of-the-generated-llvm-ir-function-that-returns-struct-type/80308
            llvm::Value* llvm_sret = this->resolver.procedure->get_sret();
            this->generate_store_expression(
                expression,
                llvm_sret,
                return_type
            );
            this->llvm_builder->CreateRetVoid();
        }
        else
        {
            llvm::Value* llvm_return_value = 
                this->generate_value_expression(
                    expression,
                    return_type
                );
            this->llvm_builder->
                CreateRet(
                    llvm_return_value
                );
        }

    }
}

void Builder::generate_GO_TO_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::GO_TO);
    assert(!operation.branches.empty());
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    std::string_view name = std::get<std::string_view>(name_expression);
    r::Label& label = this->label_table[name];
    if (label.llvm_block == nullptr)
    {
        label.llvm_block = this->create_block("name");
    }
    this->llvm_builder->CreateBr(
        label.llvm_block
    );
}

void Builder::generate_break_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::BREAK);
    if (operation.branches.empty())
    {
        llvm::BasicBlock* llvm_break_block = this->llvm_break_stack.back();
        this->llvm_builder->
            CreateBr(
                llvm_break_block
            );
    }
    else if (operation.branches.size() == 1UZ)
    {
        const r::Expression& count_expression = operation.branches.back();
        assert(std::holds_alternative<r::Literal>(count_expression));
        const r::Literal& count_literal = std::get<r::Literal>(count_expression);
        assert(count_literal.type == r::LiteralType::NUMBER);
        std::uint64_t count = r::to_number(count_literal.text);
        if (count == 0)
        {
            throw std::runtime_error("break count must not be 0.");
        }
        if (count >= this->llvm_break_stack.size())
        {
            throw std::runtime_error("break count must not be larger than break stack size.");
        }
        llvm::BasicBlock* llvm_break_block = this->llvm_break_stack[this->llvm_break_stack.size() - 1UZ - count];
        this->llvm_builder->
            CreateBr(
                llvm_break_block
            );
    }
    else
    {
        r::unreachable();
    }
}

void Builder::generate_continue_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::CONTINUE);
    if (operation.branches.empty())
    {
        llvm::BasicBlock* llvm_continue_block = this->llvm_continue_stack.back();
        this->llvm_builder->
            CreateBr(
                llvm_continue_block
            );
    }
    else if (operation.branches.size() == 1UZ)
    {
        const r::Expression& count_expression = operation.branches.back();
        assert(std::holds_alternative<r::Literal>(count_expression));
        const r::Literal& count_literal = std::get<r::Literal>(count_expression);
        assert(count_literal.type == r::LiteralType::NUMBER);
        std::uint64_t count = r::to_number(count_literal.text);
        if (count == 0)
        {
            throw std::runtime_error("continue count must not be 0.");
        }
        if (count >= this->llvm_continue_stack.size())
        {
            throw std::runtime_error("continue count must not be larger than continue stack size.");
        }
        llvm::BasicBlock* llvm_continue_block = this->llvm_continue_stack[this->llvm_break_stack.size() - 1UZ - count];
        this->llvm_builder->
            CreateBr(
                llvm_continue_block
            );
    }
    else
    {
        r::unreachable();
    }
}

void Builder::generate_label_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::LABEL);
    assert(!operation.branches.empty());
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    std::string_view name = std::get<std::string_view>(name_expression);
    r::Label& label = this->label_table[name];
    if (label.llvm_block == nullptr)
    {
        label.llvm_block = this->create_block(name);
        label.is_placed = true;
    }
    else
    {
        if (label.is_placed)
        {
            throw std::runtime_error("label of name must exist in only one location within frame.");
        }
        label.is_placed = true;
    }
    this->llvm_builder->
        CreateBr(
            label.llvm_block
        );
    this->set_current_block(label.llvm_block);

}

}