// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <utility>
#include <cassert>

namespace r {

[[noreturn]] static inline void unreachable()
{
    // assert false before calling std::unreachable(). This prevents hard to 
    // diagnose segfaults in debug builds when std::unreachable() is hit.
    assert(false);
    std::unreachable();
}

}