// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>
#include <cstddef>

namespace r {

enum class FloatingPointType
{
    UNKNOWN,
    BRAIN,
    BINARY_HALF,
    BINARY_SINGLE,
    BINARY_DOUBLE,
    BINARY_QUAD,
    PCC_DOUBLE_DOUBLE,
    FLOAT8_E5M2,
    FLOAT8_E6M2FNUZ,
    FLOAT8_E4M3FN,
    FLOAT8_E4M3FNUZ,
    FLOAT8_E4M3B11FNUZ,
    FLOAT_TF32,
    X87_DOUBLE_EXTENDED
};

std::string_view to_string(r::FloatingPointType floating_point_type);

r::FloatingPointType to_floating_point_type(std::string_view text);

std::size_t get_bit_depth(r::FloatingPointType floating_point_type);

}