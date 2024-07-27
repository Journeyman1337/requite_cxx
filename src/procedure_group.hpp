// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type.hpp>
#include <procedure_category.hpp>

#include <vector>
#include <string_view>
#include <span>

namespace r {

struct Procedure;
struct ProcedureArgument;
struct Resolver;

struct ProcedureGroup final
{
    std::string_view name{};
    r::ProcedureCategory category = r::ProcedureCategory::UNKNOWN;
    std::vector<r::Procedure*> overloads{};
    r::Type return_type{};

    void add_overload(r::Procedure& procedure, r::Resolver& resolver);
    bool get_is_empty() const noexcept;
}; 

}