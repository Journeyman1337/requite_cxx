// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <operation.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

void Cataloger::tabulate_attributes(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::ATTRIBUTES);
    const r::Expression& last_branch = operation.branches.back();
    assert(std::holds_alternative<r::Operation>(last_branch));
    const r::Operation& last_operation = std::get<r::Operation>(last_branch);
    switch (last_operation.opcode)
    {
        case r::Opcode::GLOBAL:
            this->tabulate_global(last_operation, &operation);
            break;
        case r::Opcode::OBJECT:
            this->tabulate_object(last_operation, &operation);
            break;
        case r::Opcode::PROPERTY:
            this->tabulate_property(last_operation, &operation);
            break;
        case r::Opcode::FUNCTION:
            [[fallthrough]];
        case r::Opcode::METHOD:
            [[fallthrough]];
        case r::Opcode::CONSTRUCTOR:
            [[fallthrough]];
        case r::Opcode::DESTRUCTOR:
            [[fallthrough]];
        case r::Opcode::ENTRY_POINT:
            [[fallthrough]];
        case r::Opcode::EXTERNAL_FUNCTION:
            this->tabulate_procedure(last_operation, &operation);
            break;
        default:
            r::unreachable();
    }
}

}