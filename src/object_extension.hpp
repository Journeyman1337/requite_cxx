// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <symbol_table.hpp>

#include <memory>
#include <string_view>

namespace r {

struct Object;
struct Module;
struct ExportGroup;
struct Operation;

struct ObjectExtension final
{
    std::string_view name;
    std::size_t module_symbol_i = 0UZ;
    r::Object* object;
    const r::Operation* declaration = nullptr;
    r::Module* module = nullptr;
    r::ExportGroup* export_group = nullptr;
};

}