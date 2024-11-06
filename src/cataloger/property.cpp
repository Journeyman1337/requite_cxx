// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <operation.hpp>
#include <object.hpp>
#include <property.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

void Cataloger::tabulate_property(const r::Operation& operation, const r::Operation* attributes_operation)
{
    assert(operation.opcode == r::Opcode::PROPERTY);
    assert(operation.branches.size() == 3UZ || operation.branches.size() == 2UZ);
    r::Property& property = this->resolver.add_property();
    property.module_symbol_i = this->module_symbol_i++;
    property.declaration = &operation;
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    property.name = std::get<std::string_view>(name_expression);
    property.object = this->resolver.object;
    if (attributes_operation != nullptr)
    {
        property.attributes = r::Attributes(*attributes_operation);
    }
    this->resolver.object->add_to_table(property);
}

void Cataloger::catalog(r::Property& property)
{
    assert(property.declaration != nullptr);
    assert(property.declaration->branches.size() >= 2UZ);
    const r::Expression& name_expression = property.declaration->branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    property.name = std::get<std::string_view>(name_expression);
    if (property.declaration->branches.size() == 3UZ)
    {
        const r::Expression& type_expression = property.declaration->branches.at(1UZ);
        property.type = this->resolver.resolve_type(type_expression);
        property.type.resolve_type_alias();
        property.value_i = 2UZ;
    }
    else if (property.declaration->branches.size() == 2UZ)
    {
        const r::Expression& last_expression = property.declaration->branches.back();
        property.type = this->resolver.deduce_type(last_expression); 
        property.type.resolve_type_alias();
        property.value_i = 1UZ;

    }
    else
    {
        r:unreachable();
    }
}

}