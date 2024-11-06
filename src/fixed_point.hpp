// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <integer.hpp>

#include <cstddef>
#include <cassert>

namespace r {

namespace {
constexpr bool get_is_valid_fixed_point_bit_depth(std::size_t integer_bits, std::size_t decimal_bits)
{
    return 
        r::get_is_valid_integer_bit_depth(integer_bits + decimal_bits);
}
}

struct FixedPoint final
{
    std::size_t integer_bits = 0UZ;
    std::size_t decimal_bits = 0UZ;

    constexpr FixedPoint() = default;

    constexpr FixedPoint(std::size_t integer_bits, std::size_t decimal_bits)
        : integer_bits(integer_bits)
        , decimal_bits(decimal_bits)
    {
        assert(r::get_is_valid_fixed_point_bit_depth(integer_bits, decimal_bits));
    }

    constexpr std::size_t get_bit_depth() const noexcept
    {
        return integer_bits + decimal_bits;
    }
};

constexpr bool operator==(const r::FixedPoint& lhs, const r::FixedPoint& rhs)
{
    return 
        lhs.integer_bits == rhs.integer_bits &&
        lhs.decimal_bits == rhs.decimal_bits;
}

constexpr bool operator!=(const r::FixedPoint& lhs, const r::FixedPoint& rhs)
{
    return 
        lhs.integer_bits != rhs.integer_bits ||
        lhs.decimal_bits != rhs.decimal_bits;
}

}