// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <utility>
#include <cassert>

namespace requite {

[[noreturn]] constexpr inline void unreachable() noexcept
{
    assert(false);
    std::unreachable();
}

}