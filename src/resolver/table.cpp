// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <resolver/resolver.hpp>
#include <symbol_table.hpp>
#include <module/module.hpp>
#include <export_group.hpp>
#include <object.hpp>
#include <binary.hpp>
#include <utility.hpp>

#include <ranges>
#include <cstddef>

namespace r {

void Resolver::access_table(const r::Operation& operation, bool ignore_last)
{
    assert(operation.opcode == r::Opcode::ACCESS_TABLE);
    assert(!operation.branches.empty());
    std::size_t take_count = operation.branches.size();
    if (ignore_last)
    {
        assert(operation.branches.size() > 1UZ);
        take_count--;
    }
    for (const r::Expression& branch_expression : operation.branches | std::views::take(take_count))
    {
        if (std::holds_alternative<std::string_view>(branch_expression))
        {
            std::string_view table_name = std::get<std::string_view>(branch_expression);
            r::Symbol* symbol = this->try_get_symbol(table_name);
            if (symbol == nullptr)
            {
                throw std::runtime_error("symbol not found.");
            }
            else if (std::holds_alternative<r::ExportGroup*>(*symbol))
            {
                r::ExportGroup& export_group = *std::get<r::ExportGroup*>(*symbol);
                this->table = &export_group.table;
            }
            else if (std::holds_alternative<r::Object*>(*symbol))
            {
                r::Object& object = *std::get<r::Object*>(*symbol);
                this->table = &object.table;
            }
            else
            {
                throw std::runtime_error("symbol has no table");
            } 
        }
        else if (std::holds_alternative<r::Operation>(branch_expression))
        {
            const r::Operation& branch_operation = std::get<r::Operation>(branch_expression);
            if (branch_operation.opcode == r::Opcode::ACCESS_TABLE)
            {
                this->access_table(branch_operation, false);
            }
            else if (branch_operation.opcode == r::Opcode::EXPORT_GROUP)
            {
                assert(this->export_group != nullptr);
                this->table = &this->export_group->table;
            }
            else if (branch_operation.opcode == r::Opcode::OBJECT)
            {
                assert(this->object != nullptr);
                this->table = &this->object->table;
            }
            else if (branch_operation.opcode == r::Opcode::MODULE)
            {
                assert(this->module != nullptr);
                this->table = &this->module->table;
            }
            else
            {
                r::unreachable();
            }
        }
        else
        {
            r::unreachable();
        }        
    }
}

const r::Expression& Resolver::resolve_table(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::ACCESS_TABLE);
    assert(operation.branches.size() >= 2UZ);
    this->access_table(operation, true);
    const r::Expression* last_expression_ptr = &operation.branches.back();
    while (std::holds_alternative<r::Operation>(*last_expression_ptr))
    {
        const r::Operation& last_operation = std::get<r::Operation>(*last_expression_ptr);
        if (last_operation.opcode == r::Opcode::ACCESS_TABLE)
        {
            assert(last_operation.branches.size() >= 2UZ);
            this->access_table(last_operation, true);
        }
        else
        {
            break;
        }        
    }
    return *last_expression_ptr;
}

void Resolver::reset_table()
{
    this->table = nullptr;
}

r::SymbolTable& Resolver::get_current_table()
{
    if (this->table != nullptr)
    {
        return *this->table;
    }
    if (this->object != nullptr)
    {
        return this->object->table;
    }
    else if (this->export_group != nullptr)
    {
        return this->export_group->table;
    }
    else if (this->module != nullptr)
    {
        return this->module->table;
    }
    r::unreachable();
}

r::ExportGroup& Resolver::get_created_export_group(const r::Expression& expression)
{
    if (std::holds_alternative<r::Operation>(expression))
    {
        const r::Operation& operation = std::get<r::Operation>(expression);
        assert(operation.opcode == r::Opcode::ACCESS_TABLE);
        assert(operation.branches.size() >= 2UZ);
        for (const r::Expression& table_branch : operation.branches | std::views::take(operation.branches.size() - 1UZ))
        {
            r::ExportGroup& next_group = this->get_created_export_group(table_branch);
            this->enter(next_group, *this->module);
        }
        const r::Expression& last_expression = operation.branches.back();
        return this->get_created_export_group(last_expression);
    }
    else if (std::holds_alternative<std::string_view>(expression))
    {
        std::string_view name = std::get<std::string_view>(expression);
        r::Symbol* symbol_ptr = this->try_get_symbol(name);
        if (symbol_ptr == nullptr)
        {
            r::ExportGroup& export_group = this->add_export_group();
            export_group.name = name;
            this->add_to_table(export_group);
            return export_group;
        }
        else if (!std::holds_alternative<r::ExportGroup*>(*symbol_ptr))
        {
            throw std::runtime_error("name is taken.");
        }
        return *std::get<r::ExportGroup*>(*symbol_ptr);
    }
    r::unreachable();
}

r::Symbol* Resolver::try_get_symbol(const r::Expression& expression)
{
    const r::Expression* expression_ptr = &expression;
    if (std::holds_alternative<r::Operation>(*expression_ptr))
    {
        const r::Operation& operation = std::get<r::Operation>(*expression_ptr);
        if (operation.opcode == r::Opcode::ACCESS_TABLE)
        {
            expression_ptr = &this->resolve_table(operation);
        }
        else
        {
           return nullptr;
        }
    }
    assert(std::holds_alternative<std::string_view>(*expression_ptr));
    std::string_view symbol_name = std::get<std::string_view>(*expression_ptr);
    r::Symbol* symbol_ptr = this->try_get_symbol(symbol_name);
    this->reset_table();
    return symbol_ptr;
}

r::Symbol* Resolver::try_get_symbol(std::string_view name)
{
    r::Symbol* symbol = nullptr;
    if (this->table != nullptr)
    {
        symbol = this->table->try_get_symbol(name);
    }
    if (
        symbol == nullptr &&
        this->object != nullptr    
    )
    { // try searching the object table.
        symbol = this->object->table.try_get_symbol(name);
    }
    if (
        symbol == nullptr &&
        this->export_group != nullptr    
    )
    { // try searching the export group table.
        symbol = this->export_group->table.try_get_symbol(name);
    }
    if (
        symbol == nullptr &&
        this->module != nullptr
    )
    { // try searching the module table.
        symbol = this->module->table.try_get_symbol(name);
    }
    if (symbol == nullptr)
    { // try searching the global binary table.
        symbol = this->binary->table.try_get_symbol(name);
    }
    return symbol;
}

void Resolver::add_to_table(r::Procedure& procedure)
{
    r::SymbolTable& current_table = this->get_current_table();
    current_table.add_to_table(procedure, *this);
}

void Resolver::add_to_table(r::Global& global)
{
    r::SymbolTable& current_table = this->get_current_table();
    current_table.add_to_table(global);
}

void Resolver::add_to_table(r::Object& object)
{
    r::SymbolTable& current_table = this->get_current_table();
    current_table.add_to_table(object);
}

void Resolver::add_to_table(r::TypeAlias& type_alias)
{
    r::SymbolTable& current_table = this->get_current_table();
    current_table.add_to_table(type_alias);
}

void Resolver::add_to_table(r::ExportGroup& export_group)
{
    if (this->export_group != nullptr)
    {
        this->export_group->table.add_to_table(export_group);
    }
    assert(this->binary != nullptr);
    this->binary->table.add_to_table(export_group);
}

}

