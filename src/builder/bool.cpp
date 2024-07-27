// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>

#include <cassert>

namespace r {
    
llvm::Constant* Builder::generate_true_constant_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::TRUE);
    assert(operation.branches.empty());
    this->resolver.check_type_assignable_to_type(
        r::BOOL_TYPE,
        expected_type
    );
    return
        this->llvm_builder->
            getTrue();
}

llvm::Constant* Builder::generate_false_constant_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::FALSE);
    assert(operation.branches.empty());
    this->resolver.check_type_assignable_to_type(
        r::BOOL_TYPE,
        expected_type
    );
    return
        this->llvm_builder->
            getFalse();
}

}