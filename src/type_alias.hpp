// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <operation.hpp>
#include <type.hpp>

#include <string_view>
#include <cstddef>

namespace r {

struct Module;
struct ExportGroup;
struct Object;

struct TypeAlias final
{
    std::string_view name{};
    std::size_t module_symbol_i = 0UZ;
    const r::Operation* declaration = nullptr;
    r::Module* module = nullptr;
    r::ExportGroup* export_group = nullptr;
    r::Object* object = nullptr;
    r::Type type{};
};

}