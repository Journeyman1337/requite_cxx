// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/IR/BasicBlock.h>

namespace r {

struct Label final
{
    llvm::BasicBlock* llvm_block = nullptr;
    bool is_placed = false;
};

}