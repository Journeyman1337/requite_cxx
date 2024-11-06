// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <symbol.hpp>

#include <map>
#include <string>

namespace r {

void throw_disambiguous_symbol_name();

struct Binary;
struct Resolver;
struct Procedure;

struct SymbolTable final
{
    std::map<std::string_view, r::Symbol> table{};

    void add_to_table(r::Procedure& procedure, r::Resolver& resolver);
    void add_to_table(r::Global& global); 
    void add_to_table(r::Object& object);
    void add_to_table(r::ExportGroup& export_group);
    void add_to_table(r::TypeAlias& type_alias);

    r::ProcedureGroup* try_get_procedure_group(std::string_view name);
    r::Global* try_get_global(std::string_view name);
    r::Object* try_get_object(std::string_view name);
    r::ExportGroup* try_get_export_group(std::string_view name);
    r::TypeAlias* try_get_type_alias(std::string_view name);
    r::Symbol* try_get_symbol(std::string_view name);
};


}