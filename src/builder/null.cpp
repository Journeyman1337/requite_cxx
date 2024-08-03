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

void Builder::check_is_null_value_expression(const r::Expression& expression)
{
    const auto throw_invalid_null_value =
        []()
        {
            throw std::runtime_error("variable of null type must be assigned a value of null.");
        };
    if (!std::holds_alternative<r::Operation>(expression))
    {
        throw_invalid_null_value();
    }
    const r::Operation& operation = std::get<r::Operation>(expression);
    if (operation.opcode != r::Opcode::_NULL)
    {
        throw_invalid_null_value();
    }
    assert(operation.branches.empty());
}

}