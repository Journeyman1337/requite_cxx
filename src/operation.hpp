// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <opcode.hpp>
#include <literal.hpp>

#include <string_view>
#include <vector>
#include <variant>

namespace r {

struct Operation final
{
    r::Opcode opcode = r::Opcode::UNKNOWN;
    std::vector<std::variant<std::string_view, r::Operation, r::Literal>> branches{};
};

}