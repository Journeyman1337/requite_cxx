// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <operation.hpp>
#include <module/module.hpp>
#include <utility.hpp>
#include <object.hpp>

#include <memory>
#include <cassert>
#include <ranges>

namespace r {

void Cataloger::tabulate_object(const r::Operation& operation, const r::Operation* attributes_operation)
{
    assert(operation.opcode == r::Opcode::OBJECT);
    assert(!operation.branches.empty());
    r::Object& object = this->resolver.add_object();
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    object.name = std::get<std::string_view>(name_expression);
    object.module_symbol_i = this->module_symbol_i++;
    object.export_group = this->resolver.export_group;
    object.declaration = &operation;
    object.module = this->resolver.module;
    if (attributes_operation != nullptr)
    {
        object.attributes = r::Attributes(*attributes_operation);
    }
    r::Resolver old_resolver = this->resolver;
    this->resolver.enter(object);
    for (const r::Expression& branch_expression : operation.branches | std::views::drop(1UZ))
    {
        assert(std::holds_alternative<r::Operation>(branch_expression));
        const r::Operation& branch_operation = std::get<r::Operation>(branch_expression);
        switch (branch_operation.opcode)
        {
            case r::Opcode::ATTRIBUTES:
                this->tabulate_attributes(branch_operation);
                break;
            case r::Opcode::GLOBAL:
                this->tabulate_global(branch_operation);
                break;
            case r::Opcode::METHOD:
                [[fallthrough]];
            case r::Opcode::CONSTRUCTOR:
                [[fallthrough]];
            case r::Opcode::DESTRUCTOR:
                [[fallthrough]];
            case r::Opcode::FUNCTION:
                this->tabulate_procedure(branch_operation);
                break;
            case r::Opcode::TYPE_ALIAS:
                this->tabulate_type_alias(branch_operation);
                break;
            case r::Opcode::PROPERTY:
                this->tabulate_property(branch_operation);
                break;
            default:
                r::unreachable();
        }
    }
    if (object.constructor_group.overloads.empty())
    {
        r::Procedure& constructor = this->resolver.add_procedure();
        constructor.category = r::ProcedureCategory::DEFAULT_CONSTRUCTOR;
        constructor.module_symbol_i = this->module_symbol_i++;
        constructor.return_type = r::VOID_TYPE;
        constructor.module = this->resolver.module;
        constructor.object = this->resolver.object;
        constructor.export_group = this->resolver.export_group;
        object.constructor_group.add_overload(constructor, this->resolver);

    }
    this->resolver = old_resolver;
    this->resolver.add_to_table(object);
}

void Cataloger::catalog(r::Object& object)
{
    this->resolver.enter(object);
    for (std::unique_ptr<r::Property>& property_ptr : object.properties)
    {
        r::Property& property = *property_ptr.get();
        this->catalog(property);
    }
    const r::Operation* packed_attribute = object.attributes.try_get_attribute(r::Opcode::PACKED);
    if (packed_attribute != nullptr)
    {
        assert(packed_attribute->opcode == r::Opcode::PACKED);
        assert(packed_attribute->branches.empty());
        object.is_packed = true;
    }
    else
    {
        object.is_packed = false;
    }
    this->resolver.clear();
}

}