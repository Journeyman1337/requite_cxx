// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <opcode.hpp>

namespace r {

enum class ProcedureCategory
{
    UNKNOWN,
    FUNCTION,
    METHOD,
    DEFAULT_CONSTRUCTOR,
    CONSTRUCTOR,
    DESTRUCTOR,
    ENTRY_POINT,
    EXTERNAL_FUNCTION
};

r::ProcedureCategory to_procedure_category(r::Opcode opcode) noexcept;

bool get_is_instanced(r::ProcedureCategory category) noexcept;

bool get_is_constructor(r::ProcedureCategory category) noexcept;

bool get_is_destructor(r::ProcedureCategory category) noexcept;

bool get_is_default(r::ProcedureCategory category) noexcept;

}