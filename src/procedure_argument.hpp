// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type.hpp>

#include <string_view>

namespace r {

struct ProcedureArgument final
{
    std::string_view name{};
    r::Type type{};
};

}