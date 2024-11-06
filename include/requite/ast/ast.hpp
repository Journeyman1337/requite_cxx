// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/ast/expression.hpp>

#include <string_view>
#include <vector>

namespace requite {

struct Ast final
{
    std::vector<requite::Expression> expressions;

    // parse.cpp
    bool parse(std::string_view source);
};

}