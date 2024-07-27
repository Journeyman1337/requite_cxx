// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <binary.hpp>
#include <module/module.hpp>
#include <procedure.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

#include <cassert>
#include <ranges>
#include <memory>
#include <string_view>

namespace r {

llvm::BasicBlock* Builder::create_block(std::string_view name)
{
    assert(!name.empty());
    return
        llvm::BasicBlock::Create(
            this->resolver.get_llvm_context(),
            name,
            this->resolver.procedure->llvm_function
        );
}

void Builder::set_current_block(llvm::BasicBlock* block)
{
    assert(block != nullptr);
    this->current_block = block;
    this->llvm_builder->
        SetInsertPoint(
            this->current_block
        );
}

}