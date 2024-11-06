// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

[[noreturn]] constexpr inline void unreachable() noexcept;

}

#include <requite/utility/detail/unreachable.inl>