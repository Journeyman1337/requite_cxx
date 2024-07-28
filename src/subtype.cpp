// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <subtype.hpp>

namespace r {

void Subtype::set_mutable(bool mutability) noexcept
{
    if (mutability)
    {
        qualifiers.set(r::QualifierFlag::MUTABLE);
    }
    else
    {
        qualifiers.reset(r::QualifierFlag::MUTABLE);
    }
}

void Subtype::set_volatile(bool volatility) noexcept
{
    if (volatility)
    {
        qualifiers.set(r::QualifierFlag::VOLATILE);
    }
    else
    {
        qualifiers.reset(r::QualifierFlag::VOLATILE);
    }
}

}