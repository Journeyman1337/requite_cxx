// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <procedure_category.hpp>
#include <utility.hpp>

namespace r {

r::ProcedureCategory to_procedure_category(r::Opcode opcode) noexcept
{
    switch (opcode)
    {
        case r::Opcode::FUNCTION:
            return r::ProcedureCategory::FUNCTION;
        case r::Opcode::METHOD:
            return r::ProcedureCategory::METHOD;
        case r::Opcode::CONSTRUCTOR:
            return r::ProcedureCategory::CONSTRUCTOR;
        case r::Opcode::ENTRY_POINT:
            return r::ProcedureCategory::ENTRY_POINT;
        case r::Opcode::EXTERNAL_FUNCTION:
            return r::ProcedureCategory::EXTERNAL_FUNCTION;
        case r::Opcode::DESTRUCTOR:
            return r::ProcedureCategory::DESTRUCTOR;
        default:
            r::unreachable();
    }
    r::unreachable();
}

bool get_is_instanced(r::ProcedureCategory category) noexcept
{
    return
        category == r::ProcedureCategory::METHOD ||
        category == r::ProcedureCategory::DEFAULT_CONSTRUCTOR ||
        category == r::ProcedureCategory::CONSTRUCTOR ||
        category == r::ProcedureCategory::DESTRUCTOR;
}

bool get_is_constructor(r::ProcedureCategory category) noexcept
{
    return
        category == r::ProcedureCategory::DEFAULT_CONSTRUCTOR ||
        category == r::ProcedureCategory::CONSTRUCTOR;
}

bool get_is_destructor(r::ProcedureCategory category) noexcept
{
    return
        category == r::ProcedureCategory::DESTRUCTOR;
}

bool get_is_default(r::ProcedureCategory category) noexcept
{
    return
        category == r::ProcedureCategory::DEFAULT_CONSTRUCTOR;
}

}
