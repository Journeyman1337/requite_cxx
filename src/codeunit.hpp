// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <encoding.hpp>

namespace r {

struct Codeunit final
{
    r::Encoding encoding = r::Encoding::UNKNOWN;

    constexpr Codeunit() = default;

    constexpr Codeunit(r::Encoding encoding)
        : encoding(encoding)
    {}

    constexpr std::size_t get_bit_depth() const noexcept
    {
        return r::get_codeunit_bit_depth(encoding);
    }
};

constexpr bool operator==(const r::Codeunit& lhs, const r::Codeunit& rhs)
{
    return lhs.encoding == rhs.encoding;
}

constexpr bool operator!=(const r::Codeunit& lhs, const r::Codeunit& rhs)
{
    return lhs.encoding != rhs.encoding;
}

}