// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type.hpp>
#include <attributes.hpp>

#include <llvm/IR/Value.h>
#include <llvm/IR/Instructions.h>

#include <string_view>
#include <cstddef>

namespace r {

struct Local final
{
    std::string_view name;
    r::Type type{};
    r::Attributes attributes{};
    bool has_value = false;

    llvm::AllocaInst* llvm_alloca = nullptr;
    llvm::Value* llvm_dynamic_array_size = nullptr;

    bool get_is_generated() const noexcept;
    bool get_is_no_autodestruct() const noexcept;
};

}