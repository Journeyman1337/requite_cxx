// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <break_type.hpp>
#include <operation.hpp>
#include <opcode.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::generate_bitwise_complement_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    return nullptr;
}

void Builder::generate_bitwise_complement_assignment_statement(const r::Operation& operation)
{

}

llvm::Value* Builder::generate_bitwise_complement_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    return nullptr;
}

}