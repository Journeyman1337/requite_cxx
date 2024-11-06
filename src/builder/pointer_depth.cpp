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

llvm::ConstantInt* Builder::generate_pointer_depth_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::POINTER_DEPTH);
    assert(operation.branches.empty());
    std::size_t pointer_depth = this->resolver.get_pointer_bit_depth();
    r::Type uptr_type = this->resolver.get_uptr_type();
    this->resolver.check_type_assignable_to_type(uptr_type, expected_type);
    llvm::APInt llvm_ap_int(
        pointer_depth,
        std::to_array({static_cast<std::uint64_t>(pointer_depth)})
    );
    return
        llvm::ConstantInt::get(
            this->resolver.get_llvm_context(),
            llvm_ap_int
        );
}

}