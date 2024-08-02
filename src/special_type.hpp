// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>

namespace r {

enum class SpecialType
{
    BOOL,
    BYTE,
    VARIADIC_ARGUMENTS,
    VOID,
    NULL
};

std::size_t get_bit_depth(r::SpecialType special_type);

}