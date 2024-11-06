// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <operation.hpp>
#include <literal.hpp>

#include <variant>
#include <string_view>

namespace r {

using Expression = std::variant<std::string_view, r::Operation, r::Literal>;

}