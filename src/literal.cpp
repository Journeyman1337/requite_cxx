// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <literal.hpp>

namespace r {

bool operator==(const r::Literal& lhs, const r::Literal& rhs) noexcept
{
    return
        lhs.text == rhs.text &&
        lhs.type == rhs.type;
}

bool operator!=(const r::Literal& lhs, const r::Literal& rhs) noexcept
{
    return
        lhs.text != rhs.text ||
        lhs.type != rhs.type;
}

}