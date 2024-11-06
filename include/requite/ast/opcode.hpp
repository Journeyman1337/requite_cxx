// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>

namespace requite {

enum class Opcode
{
    UNKNOWN,

    ENTRY_POINT,

    LOCAL,

    SIGNED_INTEGER,

    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,

    COPY,

    GREATER,
    LESS,
    GREATER_EQUAL,
    GREATER_LESS,
    EQUAL,
    NOT_EQUAL,
    NOT,
    AND,
    OR,

    IF,
    ELSE_IF,
    ELSE,

    GOTO,
    LABEL
};

requite::Opcode get_opcode(std::string_view name);

}

#include <requite/ast/detail/opcode.inl>

