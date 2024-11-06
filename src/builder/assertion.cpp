// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>

#include <cassert>

namespace r {

void Builder::generate_assert_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::ASSERT);
    // TODO
}

void Builder::generate_assume_statement(const r::Operation& operation)
{
    assert(!operation.branches.empty());
    assert(operation.opcode == r::Opcode::ASSUME);
    for (const r::Expression& expression : operation.branches)
    {
        r::Type type = this->resolver.deduce_type(expression, this);
        this->resolver.check_type_assignable_to_type(
            type,
            r::BOOL_TYPE
        );
        this->llvm_builder->
            CreateAssumption(
                this->
                    generate_value_expression(
                        expression,
                        r::BOOL_TYPE
                    )
            );
    }
}

void Builder::generate_unreachable_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::UNREACHABLE);
    this->llvm_builder->CreateUnreachable();
}

}