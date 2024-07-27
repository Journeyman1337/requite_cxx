// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>

#include <cassert>

namespace r {

void Builder::generate_ternary_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::QUESTION);
}

llvm::Value* Builder::generate_ternary_value_expression(const r::Operation& operation, const r::Type& type)
{
    assert(operation.opcode == r::Opcode::QUESTION);
    return nullptr;
}

void Builder::generate_ternary_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& type)
{
    assert(operation.opcode == r::Opcode::QUESTION);
}

}