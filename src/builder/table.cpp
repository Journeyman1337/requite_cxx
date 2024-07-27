// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <symbol_table.hpp>
#include <symbol_table.hpp>
#include <export_group.hpp>
#include <object.hpp>
#include <module/module.hpp>
#include <binary.hpp>
#include <utility.hpp>

#include <ranges>

namespace r {

void Builder::generate_access_table_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::ACCESS_TABLE);
    assert(operation.branches.size() >= 2UZ);
    const r::Expression& resolved_expression = this->resolver.resolve_table(operation);
    if (std::holds_alternative<r::Operation>(resolved_expression))
    {
        const r::Operation& resolved_operation = std::get<r::Operation>(resolved_expression);
        this->generate_statement(resolved_operation);
    }
    else
    {
        r::unreachable();
    }
    this->resolver.reset_table();
}

llvm::Value* Builder::generate_access_table_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::ACCESS_TABLE);
    assert(operation.branches.size() >= 2UZ);
    const r::Expression& resolved_expression = this->resolver.resolve_table(operation);
    llvm::Value* llvm_value = this->generate_value_expression(resolved_expression, expected_type);
    this->resolver.reset_table();
    return llvm_value;
}

void Builder::generate_access_table_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::ACCESS_TABLE);
    assert(operation.branches.size() >= 2UZ);
    const r::Expression& resolved_expression = this->resolver.resolve_table(operation);
    this->generate_store_expression(resolved_expression, llvm_store, expected_type);
    this->resolver.reset_table();
}

}