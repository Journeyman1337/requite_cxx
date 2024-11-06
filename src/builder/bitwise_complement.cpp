// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <break_type.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::generate_bitwise_complement_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::TILDE);
    assert(operation.branches.size() == 1UZ);
    const r::Expression& from_expression = operation.branches.front();
    r::Type deduced_type = this->resolver.deduce_type(from_expression, this);
    this->resolver.check_type_assignable_to_type(deduced_type, expected_type);
    deduced_type.clear_literals();
    if (!expected_type.get_is_integer() && !expected_type.get_is_byte())
    {
        throw std::runtime_error("invalid type.");
    }
    std::size_t bit_depth = 
        this->resolver.get_bit_depth(
            deduced_type
        );
    llvm::APInt llvm_ap_int(bit_depth, ~0ULL); // with all bits set.
    llvm::ConstantInt* llvm_constant_int = 
        llvm::ConstantInt::get(
            this->resolver.get_llvm_context(),
            llvm_ap_int
        );
    llvm::Value* llvm_value = 
        this->generate_value_expression(
            from_expression,
            deduced_type
        );
    llvm_value = 
        this->llvm_builder->
            CreateXor(
                llvm_value,
                llvm_constant_int
            ); 
    return llvm_value;
}

}