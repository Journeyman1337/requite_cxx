// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <procedure.hpp>
#include <type.hpp>
#include <binary.hpp>

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

void Builder::generate_goto_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::GOTO);
    // TODO
}

void Builder::generate_break_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::BREAK);
    llvm::BasicBlock* llvm_break_block = this->llvm_break_stack.back();
    this->llvm_builder->
        CreateBr(
            llvm_break_block
        );
}

void Builder::generate_continue_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::CONTINUE);
    llvm::BasicBlock* llvm_continue_block = this->llvm_continue_stack.back();
    this->llvm_builder->
        CreateBr(
            llvm_continue_block
        );
}

void Builder::generate_label_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::LABEL);
    // TODO
}

}