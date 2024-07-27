// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/IR/Value.h>
#include <llvm/ADT/APFloat.h>

namespace r {

struct FloatingPoint;

void initialize_llvm();

const llvm::fltSemantics& get_float_semantics(const r::FloatingPoint& floating_point);

}