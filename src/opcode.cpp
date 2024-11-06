// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <opcode.hpp>

#include <utility>
#include <unordered_map>
#include <cstddef>
#include <string_view>

namespace r {

std::string_view to_string(r::Opcode opcode)
{
    switch (opcode) 
    {
        case r::Opcode::UNKNOWN:
            return "unknown";
        case r::Opcode::ACCESS_TABLE:
            return ":";
        case r::Opcode::ACCESS_MEMBER:
            return ".";
        case r::Opcode::ACCESS_VARIADIC_ARGUMENT:
            return "access_variadic_argument";
        case r::Opcode::ADDRESS_OF:
            return "address_of";
        case r::Opcode::AND:
            return "&";
        case r::Opcode::AND_AND:
            return "&&";
        case r::Opcode::AND_EQUAL:
            return "&=";
        case r::Opcode::ARGUMENTS:
            return "arguments";
        case r::Opcode::ASSERT:
            return "assert";
        case r::Opcode::ASSUME:
            return "assume";
        case r::Opcode::AT:
            return "@";
        case r::Opcode::ATTRIBUTES:
            return "attributes";
        case r::Opcode::BANG:
            return "!";
        case r::Opcode::BANG_BANG:
            return "!!";
        case r::Opcode::BANG_EQUAL:
            return "!=";
        case r::Opcode::BIT_CAST:
            return "bit_cast";
        case r::Opcode::BREAK:
            return "break";
        case r::Opcode::CALL:
            return "call";
        case r::Opcode::CAROT:
            return "^";
        case r::Opcode::CAROT_EQUAL:
            return "^=";
        case r::Opcode::CASE:
            return "case";
        case r::Opcode::COPY_SIGN:
            return "copy_sign";
        case r::Opcode::CONDITION:
            return "condition";
        case r::Opcode::CONSTRUCT:
            return "construct";
        case r::Opcode::CONSTRUCTOR:
            return "constructor";
        case r::Opcode::CONTINUE:
            return "continue";
        case r::Opcode::DEFAULT:
            return "default";
        case r::Opcode::DESTRUCTOR:
            return "detructor";
        case r::Opcode::DESTRUCT:
            return "destruct";
        case r::Opcode::DEREFERENCE:
            return "dereference";
        case r::Opcode::DIVIDE:
            return "/";
        case r::Opcode::DIVIDE_EQUAL:
            return "/=";
        case r::Opcode::ELSE:
            return "else";
        case r::Opcode::ELSE_IF:
            return "else_if";
        case r::Opcode::END_VARIADIC_ARGUMENTS:
            return "end_variadic_arguments";
        case r::Opcode::ENTRY_POINT:
            return "entry_point";
        case r::Opcode::EMPTY:
            return "empty";
        case r::Opcode::EQUAL:
            return "=";
        case r::Opcode::EQUAL_EQUAL:
            return "==";
        case r::Opcode::EXTERNAL_FUNCTION:
            return "external_function";
        case r::Opcode::EXPORT_GROUP:
            return "export_group";
        case r::Opcode::FALL_THROUGH:
            return "fall_through";
        case r::Opcode::FALSE:
            return "false";
        case r::Opcode::FOR:
            return "for";
        case r::Opcode::FUNCTION:
            return "function";
        case r::Opcode::GO_TO:
            return "go_to";
        case r::Opcode::GREATER:
            return ">";
        case r::Opcode::HASH:
            return "#";
        case r::Opcode::GREATER_EQUAL:
            return ">=";
        case r::Opcode::GREATER_GREATER:
            return ">>";
        case r::Opcode::GLOBAL:
            return "global";
        case r::Opcode::IF:
            return "if";
        case r::Opcode::INFINITE_LOOP:
            return "infinite_loop";
        case r::Opcode::INDEX_INTO:
            return "index_into";
        case r::Opcode::IMPORT:
            return "import";
        case r::Opcode::INDETERMINATE_VALUE:
            return "indeterminate_value";
        case r::Opcode::LABEL:
            return "label";
        case r::Opcode::LESS:
            return "<";
        case r::Opcode::LESS_EQUAL:
            return "<=";
        case r::Opcode::LESS_LESS:
            return "<<";
        case r::Opcode::LOCAL:
            return "local";
        case r::Opcode::MAX:
            return "max";
        case r::Opcode::MEMORY_ALLOCATE:
            return "memory_allocate";
        case r::Opcode::MEMORY_COPY:
            return "memory_copy";
        case r::Opcode::MEMORY_FREE:
            return "memory_free";
        case r::Opcode::MEMORY_MOVE:
            return "memory_move";
        case r::Opcode::MEMORY_SET:
            return "memory_set";
        case r::Opcode::MIN:
            return "min";
        case r::Opcode::METHOD:
            return "method";
        case r::Opcode::MINUS:
            return "-";
        case r::Opcode::MINUS_EQUAL:
            return "-=";
        case r::Opcode::MODULE:
            return "module";
        case r::Opcode::MODULUS:
            return "%";
        case r::Opcode::MODULUS_EQUAL:
            return "%=";
        case r::Opcode::_NULL:
            return "null";
        case r::Opcode::OBJECT:
            return "object";
        case r::Opcode::OBJECT_EXTENSION:
            return "object_extension";
        case r::Opcode::PIPE:
            return "|";
        case r::Opcode::PIPE_EQUAL:
            return "|=";
        case r::Opcode::PIPE_PIPE:
            return "||";
        case r::Opcode::PLUS:
            return "+";
        case r::Opcode::PLUS_EQUAL:
            return "+=";
        case r::Opcode::POINTER_DEPTH:
            return "pointer_depth";
        case r::Opcode::PROPERTY:
            return "property";
        case r::Opcode::QUESTION:
            return "?";
        case r::Opcode::RETURN:
            return "return";
        case r::Opcode::SCOPE:
            return "scope";
        case r::Opcode::STAR:
            return "*";
        case r::Opcode::STAR_EQUAL:
            return "*=";
        case r::Opcode::START_VARIADIC_ARGUMENTS:
            return "start_variadic_arguments";
        case r::Opcode::SIZE_OF:
            return "size_of";
        case r::Opcode::SWITCH:
            return "switch";
        case r::Opcode::THIS:
            return "this";
        case r::Opcode::TILDE:
            return "~";
        case r::Opcode::TRUE:
            return "true";
        case r::Opcode::TRUNCATE:
            return "truncate";
        case r::Opcode::TYPE_ALIAS:
            return "type_alias";
        case r::Opcode::UNREACHABLE:
            return "unreachable";
        case r::Opcode::WHILE:
            return "while";

        // builtin types
        case r::Opcode::BUILTIN_VARIADIC_ARGUMENTS:
            return "builtin_variadic_arguments";
        case r::Opcode::BUILTIN_ARRAY:
            return "builtin_array";
        case r::Opcode::BUILTIN_BOOL:
            return "builtin_bool";
        case r::Opcode::BUILTIN_BYTE:
            return "builtin_byte";
        case r::Opcode::BUILTIN_CODEUNIT:
            return "builtin_codeunit";
        case r::Opcode::BUILTIN_FIXED_POINT:
            return "builtin_fixed_point";
        case r::Opcode::BUILTIN_FLOATING_POINT:
            return "builtin_floating_point";
        case r::Opcode::BUILTIN_INTEGER:
            return "builtin_integer";
        case r::Opcode::BUILTIN_VOID:
            return "builtin_void";
        case r::Opcode::BUILTIN_NULL:
            return "builtin_null"; 

        // attributes
        case r::Opcode::CALL_CONVENTION:
            return "calling_convention";
        case r::Opcode::PUBLIC:
            return "public";
        case r::Opcode::PRIVATE:
            return "private";
        case r::Opcode::PROTECTED:
            return "protected";
        case r::Opcode::INTERNAL:
            return "internal";
        case r::Opcode::MUTATE:
            return "mutate";
        case r::Opcode::MAY_DISCARD:
            return "may_discard";
        case r::Opcode::RUNTIME:
            return "runtime";
        case r::Opcode::NO_RETURN:
            return "no_return";
        case r::Opcode::MANGLED_NAME:
            return "mangled_name";
        case r::Opcode::NO_AUTODESTRUCT:
            return "no_autodestruct";
        case r::Opcode::PACKED:
            return "packed";
        case r::Opcode::VARIADIC_ARGUMENTS:
            return "variadic_arguments";
    }
    return "unknown";
}

r::Opcode to_opcode(std::string_view str) {
    static const std::unordered_map<std::string_view, r::Opcode> map = 
        {
            {"unknown", r::Opcode::UNKNOWN},
            {":", r::Opcode::ACCESS_TABLE},
            {".", r::Opcode::ACCESS_MEMBER},
            {"access_variadic_argument", r::Opcode::ACCESS_VARIADIC_ARGUMENT},
            {"address_of", r::Opcode::ADDRESS_OF},
            {"and", r::Opcode::AND},
            {"&&", r::Opcode::AND_AND},
            {"&=", r::Opcode::AND_EQUAL},
            {"arguments", r::Opcode::ARGUMENTS},
            {"assert", r::Opcode::ASSERT},
            {"assume", r::Opcode::ASSUME},
            {"@", r::Opcode::AT},
            {"attributes", r::Opcode::ATTRIBUTES},
            {"!", r::Opcode::BANG},
            {"!!", r::Opcode::BANG_BANG},
            {"!=", r::Opcode::BANG_EQUAL},
            {"bit_cast", r::Opcode::BIT_CAST},
            {"break", r::Opcode::BREAK},
            {"call", r::Opcode::CALL},
            {"^", r::Opcode::CAROT},
            {"^=", r::Opcode::CAROT_EQUAL},
            {"case", r::Opcode::CASE},
            {"condition", r::Opcode::CONDITION},
            {"construct", r::Opcode::CONSTRUCT},
            {"constructor", r::Opcode::CONSTRUCTOR},
            {"continue", r::Opcode::CONTINUE},
            {"copy_sign", r::Opcode::COPY_SIGN},
            {"default", r::Opcode::DEFAULT},
            {"destructor", r::Opcode::DESTRUCTOR},
            {"destruct", r::Opcode::DESTRUCT},
            {"dereference", r::Opcode::DEREFERENCE},
            {"/", r::Opcode::DIVIDE},
            {"/=", r::Opcode::DIVIDE_EQUAL},
            {"else", r::Opcode::ELSE},
            {"else_if", r::Opcode::ELSE_IF},
            {"end_variadic_arguments", r::Opcode::END_VARIADIC_ARGUMENTS},
            {"entry_point", r::Opcode::ENTRY_POINT},
            {"empty", r::Opcode::EMPTY},
            {"=", r::Opcode::EQUAL},
            {"==", r::Opcode::EQUAL_EQUAL},
            {"external_function", r::Opcode::EXTERNAL_FUNCTION},
            {"export_group", r::Opcode::EXPORT_GROUP},
            {"fall_through", r::Opcode::FALL_THROUGH},
            {"false", r::Opcode::FALSE},
            {"for", r::Opcode::FOR},
            {"function", r::Opcode::FUNCTION},
            {"global", r::Opcode::GLOBAL},
            {"go_to", r::Opcode::GO_TO},
            {">", r::Opcode::GREATER},
            {">=", r::Opcode::GREATER_EQUAL},
            {">>", r::Opcode::GREATER_GREATER},
            {"#", r::Opcode::HASH},
            {"if", r::Opcode::IF},
            {"index_into", r::Opcode::INDEX_INTO},
            {"infinite_loop", r::Opcode::INFINITE_LOOP},
            {"indeterminate_value", r::Opcode::INDETERMINATE_VALUE},
            {"import", r::Opcode::IMPORT},
            {"label", r::Opcode::LABEL},
            {"<", r::Opcode::LESS},
            {"<=", r::Opcode::LESS_EQUAL},
            {"<<", r::Opcode::LESS_LESS},
            {"local", r::Opcode::LOCAL},
            {"max", r::Opcode::MAX},
            {"memory_allocate", r::Opcode::MEMORY_ALLOCATE},
            {"memory_free", r::Opcode::MEMORY_FREE},
            {"memory_copy", r::Opcode::MEMORY_COPY},
            {"memory_move", r::Opcode::MEMORY_MOVE},
            {"memory_set", r::Opcode::MEMORY_SET},
            {"min", r::Opcode::MIN},
            {"method", r::Opcode::METHOD},
            {"-", r::Opcode::MINUS},
            {"-=", r::Opcode::MINUS_EQUAL},
            {"module", r::Opcode::MODULE},
            {"%", r::Opcode::MODULUS},
            {"%=", r::Opcode::MODULUS_EQUAL},
            {"null", r::Opcode::_NULL},
            {"object", r::Opcode::OBJECT},
            {"object_extension", r::Opcode::OBJECT_EXTENSION},
            {"|", r::Opcode::PIPE},
            {"|=", r::Opcode::PIPE_EQUAL},
            {"||", r::Opcode::PIPE_PIPE},
            {"+", r::Opcode::PLUS},
            {"+=", r::Opcode::PLUS_EQUAL},
            {"pointer_depth", r::Opcode::POINTER_DEPTH},
            {"property", r::Opcode::PROPERTY},
            {"?", r::Opcode::QUESTION},
            {"return", r::Opcode::RETURN},
            {"scope", r::Opcode::SCOPE},
            {"size_of", r::Opcode::SIZE_OF},
            {"*", r::Opcode::STAR},
            {"*=", r::Opcode::STAR_EQUAL},
            {"start_variadic_arguments", r::Opcode::START_VARIADIC_ARGUMENTS},
            {"switch", r::Opcode::SWITCH},
            {"this", r::Opcode::THIS},
            {"~", r::Opcode::TILDE},
            {"true", r::Opcode::TRUE},
            {"truncate", r::Opcode::TRUNCATE},
            {"type_alias", r::Opcode::TYPE_ALIAS},
            {"unreachable", r::Opcode::UNREACHABLE},
            {"while", r::Opcode::WHILE},

            // builtin types
            {"builtin_variadic_arguments", r::Opcode::BUILTIN_VARIADIC_ARGUMENTS},
            {"builtin_array", r::Opcode::BUILTIN_ARRAY},
            {"builtin_bool", r::Opcode::BUILTIN_BOOL},
            {"builtin_byte", r::Opcode::BUILTIN_BYTE},
            {"builtin_codeunit", r::Opcode::BUILTIN_CODEUNIT},
            {"builtin_fixed_point", r::Opcode::BUILTIN_FIXED_POINT},
            {"builtin_floating_point", r::Opcode::BUILTIN_FLOATING_POINT},
            {"builtin_integer", r::Opcode::BUILTIN_INTEGER},
            {"builtin_void", r::Opcode::BUILTIN_VOID},
            {"builtin_null", r::Opcode::BUILTIN_NULL},

            // attributes
            {"calling_convention", r::Opcode::CALL_CONVENTION},
            {"public", r::Opcode::PUBLIC},
            {"private", r::Opcode::PRIVATE},
            {"protected", r::Opcode::PROTECTED},
            {"internal", r::Opcode::INTERNAL},
            {"mutate", r::Opcode::MUTATE},
            {"may_discard", r::Opcode::MAY_DISCARD},
            {"runtime", r::Opcode::RUNTIME},
            {"no_return", r::Opcode::NO_RETURN},
            {"mangled_name", r::Opcode::MANGLED_NAME},
            {"no_autodestruct", r::Opcode::NO_AUTODESTRUCT},
            {"packed", r::Opcode::PACKED},
            {"variadic_arguments", r::Opcode::VARIADIC_ARGUMENTS}
        };
    auto it = map.find(str);
    if (it != map.end()) 
    {
        return it->second;
    }
    return r::Opcode::UNKNOWN;
}

bool get_is_global_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::GLOBAL ||
        opcode == r::Opcode::OBJECT ||
        opcode == r::Opcode::OBJECT_EXTENSION ||
        opcode == r::Opcode::FUNCTION ||
        opcode == r::Opcode::EXTERNAL_FUNCTION ||
        opcode == r::Opcode::EXPORT_GROUP ||
        opcode == r::Opcode::ENTRY_POINT ||
        opcode == r::Opcode::ATTRIBUTES;
}

bool get_is_global_with_attributes_opcode(r::Opcode opcode)
{
        return
        opcode == r::Opcode::GLOBAL ||
        opcode == r::Opcode::OBJECT ||
        opcode == r::Opcode::FUNCTION ||
        opcode == r::Opcode::EXTERNAL_FUNCTION ||
        opcode == r::Opcode::EXPORT_GROUP ||
        opcode == r::Opcode::ENTRY_POINT;
}

bool get_is_object_member_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::GLOBAL ||
        opcode == r::Opcode::PROPERTY ||
        opcode == r::Opcode::CONSTRUCTOR ||
        opcode == r::Opcode::METHOD ||
        opcode == r::Opcode::ATTRIBUTES;
}

bool get_is_object_member_with_attributes_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::GLOBAL ||
        opcode == r::Opcode::PROPERTY ||
        opcode == r::Opcode::CONSTRUCTOR ||
        opcode == r::Opcode::METHOD;
}

bool get_is_object_extension_member_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::GLOBAL ||
        opcode == r::Opcode::CONSTRUCTOR ||
        opcode == r::Opcode::METHOD ||
        opcode == r::Opcode::ATTRIBUTES;
}

bool get_is_object_extension_member_with_attributes_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::GLOBAL ||
        opcode == r::Opcode::CONSTRUCTOR ||
        opcode == r::Opcode::METHOD;
}

bool get_is_variable_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::LOCAL ||
        opcode == r::Opcode::GLOBAL ||
        opcode == r::Opcode::PROPERTY; 
}

bool get_is_math_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::PLUS ||
        opcode == r::Opcode::MINUS ||
        opcode == r::Opcode::STAR ||
        opcode == r::Opcode::DIVIDE ||
        opcode == r::Opcode::MODULUS;
}

bool get_is_logical_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::AND_AND ||
        opcode == r::Opcode::PIPE_PIPE ||
        opcode == r::Opcode::BANG;
}

bool get_is_comparison_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::GREATER ||
        opcode == r::Opcode::GREATER_EQUAL ||
        opcode == r::Opcode::LESS ||
        opcode == r::Opcode::LESS_EQUAL ||
        opcode == r::Opcode::EQUAL_EQUAL ||
        opcode == r::Opcode::BANG_EQUAL;
}

bool get_is_procedure_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::FUNCTION ||
        opcode == r::Opcode::METHOD ||
        opcode == r::Opcode::CONSTRUCTOR ||
        opcode == r::Opcode::ENTRY_POINT ||
        opcode == r::Opcode::EXTERNAL_FUNCTION ||
        opcode == r::Opcode::DESTRUCTOR;
}

bool get_is_attribute_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::CALL_CONVENTION ||
        opcode == r::Opcode::PUBLIC ||
        opcode == r::Opcode::PRIVATE ||
        opcode == r::Opcode::PROTECTED ||
        opcode == r::Opcode::INTERNAL ||
        opcode == r::Opcode::MUTATE ||
        opcode == r::Opcode::MAY_DISCARD ||
        opcode == r::Opcode::RUNTIME ||
        opcode == r::Opcode::NO_RETURN ||
        opcode == r::Opcode::NO_AUTODESTRUCT ||
        opcode == r::Opcode::MANGLED_NAME ||
        opcode == r::Opcode::PACKED ||
        opcode == r::Opcode::VARIADIC_ARGUMENTS;
}

bool get_opcode_returns_bool(r::Opcode opcode)
{
    return
        r::get_is_logical_opcode(opcode) ||
        r::get_is_comparison_opcode(opcode) ||
        opcode == r::Opcode::TRUE ||
        opcode == r::Opcode::FALSE;
}

bool get_is_assignment_opcode(r::Opcode opcode)
{
    return
        opcode == r::Opcode::EQUAL ||
        opcode == r::Opcode::PLUS_EQUAL ||
        opcode == r::Opcode::MINUS_EQUAL ||
        opcode == r::Opcode::STAR_EQUAL ||
        opcode == r::Opcode::DIVIDE_EQUAL ||
        opcode == r::Opcode::MODULUS_EQUAL;
}

}