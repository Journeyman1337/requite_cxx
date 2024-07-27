// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <operation.hpp>
#include <utility.hpp>

namespace r {

void Cataloger::tabulate(const r::Operation& operation)
{
    switch (operation.opcode)
    {
        case r::Opcode::ATTRIBUTES:
            this->tabulate_attributes(operation);
            break;
        case r::Opcode::EXPORT_GROUP:
            this->tabulate_export_group(operation);
            break;
        case r::Opcode::GLOBAL:
            this->tabulate_global(operation);
            break;
        case r::Opcode::OBJECT_EXTENSION:
            // this is tabulated in a the next stage.
            break;
        case r::Opcode::OBJECT:
            this->tabulate_object(operation);
            break;
        case r::Opcode::PROPERTY:
            this->tabulate_property(operation);
            break;
        case r::Opcode::FUNCTION:
            [[fallthrough]];
        case r::Opcode::METHOD:
            [[fallthrough]];
        case r::Opcode::CONSTRUCTOR:
            [[fallthrough]];
        case r::Opcode::ENTRY_POINT:
            this->tabulate_procedure(operation);
            break;
        case r::Opcode::TYPE_ALIAS:
            this->tabulate_type_alias(operation);
            break;
        default:
            r::unreachable();
    }
}

void Cataloger::tabulate_extensions(const r::Operation& operation)
{
    switch (operation.opcode)
    {
        case r::Opcode::ATTRIBUTES:
            break;
        case r::Opcode::EXPORT_GROUP:
            break;
        case r::Opcode::GLOBAL:
            break;
        case r::Opcode::OBJECT_EXTENSION:
            this->tabulate_object_extension(operation);
            break;
        case r::Opcode::OBJECT:
            break;
        case r::Opcode::PROPERTY:
            break;
        case r::Opcode::FUNCTION:
            break;
        case r::Opcode::METHOD:
            break;
        case r::Opcode::CONSTRUCTOR:
            break;
        case r::Opcode::ENTRY_POINT:
            break;
        default:
            r::unreachable();
    }
}

}