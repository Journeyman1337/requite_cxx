// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <symbol_table.hpp>
#include <procedure.hpp>
#include <global.hpp>
#include <object.hpp>
#include <export_group.hpp>
#include <procedure_group.hpp>
#include <type_alias.hpp>
#include <binary.hpp>
#include <resolver/resolver.hpp>
#include <utility.hpp>

#include <stdexcept>
#include <utility>

namespace r {

void throw_disambiguous_symbol_name()
{
    throw std::runtime_error("name must be disambiguous.");
}

void SymbolTable::add_to_table(r::Procedure& procedure, r::Resolver& resolver)
{
    r::ProcedureGroup* procedure_group_ptr = nullptr;
    if (this->table.contains(procedure.name))
    {
        r::Symbol& symbol = this->table.at(procedure.name);
        if (!std::holds_alternative<r::ProcedureGroup*>(symbol))
        {
            r::throw_disambiguous_symbol_name();
        }
        procedure_group_ptr = std::get<r::ProcedureGroup*>(symbol);
    }
    else
    {
        procedure_group_ptr = &resolver.add_procedure_group();
        this->table[procedure.name] = procedure_group_ptr;
    }
    procedure_group_ptr->add_overload(procedure, resolver);
}

void SymbolTable::add_to_table(r::Global& global)
{
    if (this->table.contains(global.name))
    {
        r::throw_disambiguous_symbol_name();
    }
    this->table[global.name] = &global;
}

void SymbolTable::add_to_table(r::Object& object)
{
    if (this->table.contains(object.name))
    {
        r::throw_disambiguous_symbol_name();
    }
    this->table[object.name] = &object;    
}

void SymbolTable::add_to_table(r::ExportGroup& export_group)
{
    if (this->table.contains(export_group.name))
    {
        r::throw_disambiguous_symbol_name();
    }
    this->table[export_group.name] = &export_group; 
}

void SymbolTable::add_to_table(r::TypeAlias& type_alias)
{
    if (this->table.contains(type_alias.name))
    {
        r::throw_disambiguous_symbol_name();
    }
    this->table[type_alias.name] = &type_alias; 
}

r::ProcedureGroup* SymbolTable::try_get_procedure_group(std::string_view name)
{
    if (!this->table.contains(name))
    {
        return nullptr;
    }
    r::Symbol& symbol = this->table.at(name);
    if (!std::holds_alternative<r::ProcedureGroup*>(symbol))
    {
        return nullptr;
    }
    return std::get<r::ProcedureGroup*>(symbol);
}

r::Global* SymbolTable::try_get_global(std::string_view name)
{
    if (!this->table.contains(name))
    {
        return nullptr;
    }
    r::Symbol& symbol = this->table.at(name);
    if (!std::holds_alternative<r::Global*>(symbol))
    {
        return nullptr;
    }
    return std::get<r::Global*>(symbol);
}

r::Object* SymbolTable::try_get_object(std::string_view name)
{
    if (!this->table.contains(name))
    {
        return nullptr;
    }
    r::Symbol& symbol = this->table.at(name);
    if (!std::holds_alternative<r::Object*>(symbol))
    {
        return nullptr;
    }
    return std::get<r::Object*>(symbol);
}

r::ExportGroup* SymbolTable::try_get_export_group(std::string_view name)
{
    if (!this->table.contains(name))
    {
        return nullptr;
    }
    r::Symbol& symbol = this->table.at(name);
    if (!std::holds_alternative<r::ExportGroup*>(symbol))
    {
        return nullptr;
    }
    return std::get<r::ExportGroup*>(symbol);
}

r::TypeAlias* SymbolTable::try_get_type_alias(std::string_view name)
{
    if (!this->table.contains(name))
    {
        return nullptr;
    }
    r::Symbol& symbol = this->table.at(name);
    if (!std::holds_alternative<r::TypeAlias*>(symbol))
    {
        return nullptr;
    }
    return std::get<r::TypeAlias*>(symbol);
}

r::Symbol* SymbolTable::try_get_symbol(std::string_view name)
{
    if (!this->table.contains(name))
    {
        return nullptr;
    }
    return &this->table.at(name);
}

}