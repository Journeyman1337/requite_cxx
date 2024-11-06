// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <literal_type.hpp>

#include <string_view>

namespace r {

struct Literal final
{
    std::string_view text;
    r::LiteralType type;
};

bool operator==(const r::Literal& lhs, const r::Literal& rhs) noexcept;
bool operator!=(const r::Literal& lhs, const r::Literal& rhs) noexcept;

}
