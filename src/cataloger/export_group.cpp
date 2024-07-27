// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <operation.hpp>
#include <utility.hpp>

#include <cassert>
#include <ranges>

namespace r {

void Cataloger::tabulate_export_group(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::EXPORT_GROUP);
    assert(!operation.branches.empty());
    if (operation.branches.size() == 1UZ)
    {
        return;
    }
    const r::Expression& name_expression = operation.branches.front();
    r::ExportGroup& export_group = this->resolver.get_created_export_group(name_expression);
    r::Resolver old_resolver = this->resolver;
    this->resolver.enter(export_group, *old_resolver.module);
    for (const r::Expression& branch_expression : operation.branches | std::views::drop(1UZ))
    {
        assert(std::holds_alternative<r::Operation>(branch_expression));
        const r::Operation& branch_operation = std::get<r::Operation>(branch_expression);
        switch (branch_operation.opcode)
        {
            case r::Opcode::ATTRIBUTES:
                this->tabulate_attributes(branch_operation);
                break;
            case r::Opcode::EXPORT_GROUP:
                this->tabulate_export_group(branch_operation);
                break;
            case r::Opcode::GLOBAL:
                this->tabulate_global(branch_operation);
                break;
            case r::Opcode::PROPERTY:
                this->tabulate_property(branch_operation);
                break;
            case r::Opcode::FUNCTION:
                [[fallthrough]];
            case r::Opcode::METHOD:
                [[fallthrough]];
            case r::Opcode::CONSTRUCTOR:
                [[fallthrough]];
            case r::Opcode::ENTRY_POINT:
                this->tabulate_procedure(branch_operation);
                break;
            case r::Opcode::TYPE_ALIAS:
                this->tabulate_type_alias(branch_operation);
                break;
            default:
                r::unreachable();
        }
    }
    this->resolver = old_resolver;
}

}