// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <type_alias.hpp>
#include <operation.hpp>
#include <utility.hpp>

#include <cassert>
#include <ranges>

namespace r {

void Cataloger::tabulate_type_alias(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::TYPE_ALIAS);
    r::TypeAlias& type_alias = this->resolver.add_type_alias();
    type_alias.module_symbol_i = this->module_symbol_i++;
    type_alias.declaration = &operation;
    assert(operation.branches.size() == 2UZ);
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    type_alias.name = std::get<std::string_view>(name_expression);
    type_alias.module = this->resolver.module;
    type_alias.export_group = this->resolver.export_group;
    this->resolver.add_to_table(type_alias);
}

void Cataloger::catalog(r::TypeAlias& type_alias)
{
    this->resolver.enter(type_alias);
    assert(type_alias.declaration->branches.size() == 2UZ);
    const r::Expression& type_expression = type_alias.declaration->branches.back();
    type_alias.type = this->resolver.resolve_type(type_expression);
}

}