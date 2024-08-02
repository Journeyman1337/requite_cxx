// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>

namespace r {

llvm::Constant* Builder::generate_null_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::_NULL);
    assert(operation.branches.empty());
    if (!expected_type.get_is_pointer())
    {
        throw std::runtime_error("null value must be assigned to pointer.");
    }
    llvm::Type* llvm_type = 
        this->resolver.get_llvm_type(
            expected_type
        );
    assert(llvm_type->isPointerTy());
    llvm::PointerType* llvm_pointer_type = 
        llvm::dyn_cast<llvm::PointerType>(
            llvm_type
        );
    return
        llvm::ConstantPointerNull::get(
            llvm_pointer_type
        );
}

}