// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>

namespace r {

enum class Opcode
{
    UNKNOWN,
    ACCESS_TABLE,
    ACCESS_MEMBER,
    ACCESS_VARIADIC_ARGUMENT,
    ADDRESS_OF,
    AND,
    AND_AND,
    AND_EQUAL,
    ARGUMENTS,
    ASSERT,
    ASSUME,
    AT,
    ATTRIBUTES,
    BANG,
    BANG_BANG,
    BANG_EQUAL,
    BIT_CAST,
    BREAK,
    CALL,
    CAROT,
    CAROT_EQUAL,
    CASE,
    CONDITION,
    CONSTRUCT,
    CONSTRUCTOR,
    CONTINUE,
    COPY_SIGN,
    DEFAULT,
    DESTRUCTOR,
    DESTRUCT,
    DEREFERENCE,
    DIVIDE,
    DIVIDE_EQUAL,
    ELSE,
    ELSE_IF,
    END_VARIADIC_ARGUMENTS,
    ENTRY_POINT,
    EMPTY,
    EQUAL,
    EQUAL_EQUAL,
    EXPORT_GROUP,
    EXTERNAL_FUNCTION,
    FALL_THROUGH,
    FALSE,
    FOR,
    FUNCTION,
    GLOBAL,
    GO_TO,
    GREATER,
    GREATER_EQUAL,
    GREATER_GREATER,
    HASH,
    IF,
    IMPORT,
    INDETERMINATE_VALUE,
    INDEX_INTO,
    INFINITE_LOOP,
    LABEL,
    LESS,
    LESS_EQUAL,
    LESS_LESS,
    LOCAL,
    MAX,
    MEMORY_ALLOCATE,
    MEMORY_COPY,
    MEMORY_FREE,
    MEMORY_MOVE,
    MEMORY_SET,
    METHOD,
    MIN,
    MINUS,
    MINUS_EQUAL,
    MODULE,
    MODULUS,
    MODULUS_EQUAL,
    OBJECT,
    OBJECT_EXTENSION,
    PIPE,
    PIPE_EQUAL,
    PIPE_PIPE,
    PLUS,
    PLUS_EQUAL,
    POINTER_SIZE,
    PROPERTY,
    RETURN,
    QUESTION,
    SCOPE,
    SIZE_OF,
    STAR,
    STAR_EQUAL,
    START_VARIADIC_ARGUMENTS,
    SWITCH,
    THIS,
    TILDE,
    TRUE,
    TRUNCATE,
    TYPE_ALIAS,
    UNREACHABLE,
    WHILE,

    // builtin types
    BUILTIN_VARIADIC_ARGUMENTS,
    BUILTIN_ARRAY,
    BUILTIN_BOOL,
    BUILTIN_BYTE,
    BUILTIN_CODEUNIT,
    BUILTIN_FIXED_POINT,
    BUILTIN_FLOATING_POINT,
    BUILTIN_INTEGER,
    BUILTIN_VOID,

    // attributes
    CALL_CONVENTION,
    PUBLIC,
    PRIVATE,
    PROTECTED,
    INTERNAL,
    MUTATE,
    MAY_DISCARD,
    RUNTIME,
    NO_RETURN,
    MANGLED_NAME,
    NO_AUTODESTRUCT,
    PACKED,
    VARIADIC_ARGUMENTS
};

std::string_view to_string(r::Opcode opcode);

r::Opcode to_opcode(std::string_view string);

bool get_is_global_opcode(r::Opcode opcode);

bool get_is_global_with_attributes_opcode(r::Opcode opcode);

bool get_is_object_member_opcode(r::Opcode opcode);

bool get_is_object_member_with_attributes_opcode(r::Opcode opcode);

bool get_is_object_extension_member_opcode(r::Opcode opcode);

bool get_is_object_extension_member_with_attributes_opcode(r::Opcode opcode);

bool get_is_variable_opcode(r::Opcode opcode);

bool get_is_math_opcode(r::Opcode opcode);

bool get_is_logical_opcode(r::Opcode opcode);

bool get_is_comparison_opcode(r::Opcode opcode);

bool get_is_procedure_opcode(r::Opcode opcode);

bool get_is_attribute_opcode(r::Opcode opcode);

bool get_opcode_returns_bool(r::Opcode opcode);

bool get_is_assignment_opcode(r::Opcode opcode);

}