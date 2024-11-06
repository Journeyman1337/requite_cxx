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

namespace r {

void Builder::initialize(r::Binary& binary)
{
    this->resolver.enter(binary);
    this->llvm_builder = 
        std::make_unique<llvm::IRBuilder<>>(
            this->resolver.get_llvm_context()
        );
}

bool Builder::get_is_initialized() const noexcept
{
    return this->llvm_builder.get() != nullptr;
}

void Builder::terminate() noexcept
{
    this->llvm_builder.release();
}

}