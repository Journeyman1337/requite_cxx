// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/ast/expression.hpp>

#include <vector>

namespace requite {

struct Ast final
{
    std::vector<requite::Expression> expressions;
};

}