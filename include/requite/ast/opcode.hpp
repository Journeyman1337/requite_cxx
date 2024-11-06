// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

enum class Opcode
{
    UNKNOWN,

    ENTRY_POINT,
    FUNCTION,
    CALL,
    EXTERNAL_FUNCTION,

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

}