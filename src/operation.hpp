// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <opcode.hpp>
#include <literal.hpp>

#include <llvm/ADT/SmallVector.h>

#include <string_view>
#include <variant>
#include <vector>

namespace r {

struct Operation final
{
    r::Opcode opcode = r::Opcode::UNKNOWN;
    std::vector<std::variant<std::string_view, r::Operation, r::Literal>> branches{};
};

}