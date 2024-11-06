// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/ast/operation.hpp>
#include <requite/ast/literal.hpp>
#include <requite/ast/identifier.hpp>
#include <requite/ast/expression_type.hpp>

#include <vector>

namespace requite {

struct Expression final
{
    requite::ExpressionType type = requite::ExpressionType::UNKNOWN;

    requite::Operation operation;
    requite::Literal literal;
    requite::Identifier identifier;    
};

}