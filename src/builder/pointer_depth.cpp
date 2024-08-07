// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <type.hpp>

#include <llvm/ADT/APInt.h>
#include <llvm/IR/Constants.h>

#include <stdexcept>
#include <cstddef>
#include <cassert>
#include <cstdint>

namespace r {

llvm::Constant* Builder::generate_pointer_depth_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::POINTER_SIZE);
    assert(operation.branches.empty());
    if (!expected_type.get_is_integer())
    {
        throw std::runtime_error("pointer_depth must be assigned to integer type.");
    }
    r::Integer expected_integer = std::get<r::Integer>(expected_type.root);
    std::size_t pointer_depth = this->resolver.get_pointer_bit_depth();
    llvm::APInt llvm_ap_int(
        expected_integer.bit_depth,
        std::to_array({static_cast<std::uint64_t>(pointer_depth)})
    );
    return
        llvm::ConstantInt::get(
            this->resolver.get_llvm_context(),
            llvm_ap_int
        );
}

}