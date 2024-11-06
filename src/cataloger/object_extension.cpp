// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <operation.hpp>
#include <module/module.hpp>
#include <object_extension.hpp>
#include <symbol.hpp>
#include <symbol_table.hpp>
#include <utility.hpp>

#include <string_view>
#include <cassert>
#include <ranges>

namespace r {

void Cataloger::tabulate_object_extension(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::OBJECT_EXTENSION);
    assert(!operation.branches.empty());
    r::ObjectExtension& object_extension = this->resolver.add_object_extension();
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    object_extension.name = std::get<std::string_view>(name_expression);
    object_extension.module_symbol_i = this->module_symbol_i++;
    object_extension.declaration = &operation;
    object_extension.module = this->resolver.module;
    object_extension.export_group = this->resolver.export_group;
    r::Symbol* object_symbol = this->resolver.try_get_symbol(object_extension.name);
    assert(object_symbol != nullptr);
    assert(std::holds_alternative<r::Object*>(*object_symbol));
    object_extension.object = std::get<r::Object*>(*object_symbol);
    r::Resolver old_resolver = this->resolver;
    this->resolver.enter(*object_extension.object);
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
                this->tabulate_procedure(operation);
                break;
            default:
                r::unreachable();
        }
    }
    this->resolver = old_resolver;
}

}