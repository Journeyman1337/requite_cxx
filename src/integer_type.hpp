// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>

namespace r {

enum class IntegerType
{
    UNKNOWN,
    SIGNED,
    UNSIGNED
};

std::string_view to_string(r::IntegerType integer_type);

r::IntegerType to_integer_type(std::string_view text);

}