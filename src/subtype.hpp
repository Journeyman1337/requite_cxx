// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <qualifiers.hpp>

#include <cstddef>

namespace r {

struct Subtype final
{
    r::QualifierFlagSet qualifiers{};

    // must not be 0 for arrays. no unbounded arrays! (yet)
    std::size_t array_size = 0UZ;

    void set_mutable(bool mutability) noexcept;
    void set_volatile(bool volatility) noexcept;
};

}