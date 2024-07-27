// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <integer_type.hpp>

#include <cstddef>
#include <cassert>

namespace r {

namespace {
constexpr bool get_is_valid_integer_bit_depth(std::size_t bit_depth)
{
    return 
        bit_depth != 0UZ &&
        (bit_depth & (bit_depth - 1UZ)) == 0UZ;
}
}

struct Integer final
{
    r::IntegerType type = r::IntegerType::UNKNOWN;
    std::size_t bit_depth = 0UZ;

    constexpr Integer() = default;

    constexpr Integer(r::IntegerType type, std::size_t bit_depth)
        : type(type)
        , bit_depth(bit_depth)
    {
        assert(r::get_is_valid_integer_bit_depth(bit_depth));
    }
};

constexpr bool operator==(const r::Integer& lhs, const r::Integer& rhs)
{
    return 
        lhs.type == rhs.type &&
        lhs.bit_depth == rhs.bit_depth;
}

constexpr bool operator!=(const r::Integer& lhs, const r::Integer& rhs)
{
    return 
        lhs.type != rhs.type ||
        lhs.bit_depth != rhs.bit_depth;
}


}