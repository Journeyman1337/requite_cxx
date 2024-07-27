// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <floating_point_type.hpp>

namespace r {

struct FloatingPoint final
{
    r::FloatingPointType type = r::FloatingPointType::UNKNOWN;

    constexpr FloatingPoint() = default;

    constexpr FloatingPoint(r::FloatingPointType type)
        : type(type)
    {}

    constexpr std::size_t get_bit_depth() const noexcept
    {
        return r::get_bit_depth(this->type);
    }
};

constexpr bool operator==(const r::FloatingPoint& lhs, const r::FloatingPoint& rhs)
{
    return lhs.type == rhs.type;
}

constexpr bool operator!=(const r::FloatingPoint& lhs, const r::FloatingPoint& rhs)
{
    return lhs.type != rhs.type;
}

}