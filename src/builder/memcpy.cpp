// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>

#include <llvm/IR/Value.h>

#include <cstddef>

namespace r {

void Builder::generate_memcpy_static(llvm::Value* llvm_source, llvm::Value* llvm_dest, std::size_t size)
{
    // TODO with IRBuilder::CreateMemcpyInline
}

void Builder::generate_memcpy(llvm::Value* llvm_source, llvm::Value* llvm_dest, llvm::Value* llvm_size)
{
    // TODO with IRBuilder::CreateMemcpy
}

}