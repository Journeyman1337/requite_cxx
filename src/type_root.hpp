// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include "codeunit.hpp"
#include <integer.hpp>
#include <floating_point.hpp>
#include <fixed_point.hpp>
#include <special_type.hpp>

#include <variant>

namespace r {

struct Object;
struct TypeAlias;

using TypeRoot =
    std::variant<
        std::monostate,
        r::Codeunit,
        r::Integer,
        r::FloatingPoint,
        r::FixedPoint,
        r::SpecialType,
        r::Object*,
        r::TypeAlias*
    >;

}