// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace r {

enum class BreakType
{
    NONE,
    BREAK,
    CONTINUE,
    FALL_THROUGH,
    RETURN,
    GO_TO
};

}