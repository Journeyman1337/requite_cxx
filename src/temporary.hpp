// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type.hpp>

#include <llvm/IR/Instructions.h>

namespace r {

struct Temporary final
{
    r::Type type{};
    llvm::AllocaInst* llvm_alloca = nullptr;
    llvm::Value* llvm_dynamic_array_size = nullptr;
};

}