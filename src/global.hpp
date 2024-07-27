// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type.hpp>
#include <attributes.hpp>

#include <string_view>

namespace r {

struct Operation;
struct Object;
struct ExportGroup;
struct Procedure;
struct Module;
struct SymbolTable;
struct Builder;

struct Global final
{
    std::string_view name{};
    std::size_t module_symbol_i = 0UZ;
    r::ExportGroup* export_group = nullptr;
    r::Object* object = nullptr;
    r::Procedure* procedure = nullptr;
    r::Type type{};
    const r::Operation* declaration{};
    r::Module* module = nullptr;
    std::size_t value_i = 0UZ;
    r::Attributes attributes{};
};

}