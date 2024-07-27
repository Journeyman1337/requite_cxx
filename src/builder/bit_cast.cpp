// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <binary.hpp>

#include <cassert>

namespace r {
    
llvm::Value* Builder::generate_bit_cast_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::BIT_CAST);
    assert(operation.branches.size() == 2UZ);
    const r::Expression& dest_type_expression = operation.branches.front();
    r::Type dest_type = this->resolver.resolve_type(dest_type_expression);
    this->resolver.check_type_assignable_to_type(dest_type, expected_type);
    const r::Expression& source_expression = operation.branches.back();
    r::Type src_type = this->resolver.deduce_type(source_expression, this);
    // if (src_type.get_byte_size() != dest_type.get_byte_size())
    // {
    //     throw std::runtime_error("must bit cast between types of same byte sizes.");
    // }
    llvm::Value* llvm_cast_value =
        this->generate_value_expression(
            operation.branches.back(),
            src_type
        );
    llvm::Type* llvm_type =
        this->resolver.
            get_llvm_type(
                dest_type
            );
    return
        this->llvm_builder->
            CreateBitCast(
                llvm_cast_value,
                llvm_type
            );
}

void Builder::generate_bit_cast_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& type)
{
    assert(operation.opcode == r::Opcode::BIT_CAST);
    // TODO use memcpy
}

}