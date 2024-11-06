// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

namespace requite {

requite::Opcode get_opcode(std::string_view name)
{
    if (name == "UNKNOWN") return requite::Opcode::UNKNOWN;
    else if (name == "ENTRY_POINT") return requite::Opcode::ENTRY_POINT;
    else if (name == "LOCAL") return requite::Opcode::LOCAL;
    else if (name == "SIGNED_INTEGER") return requite::Opcode::SIGNED_INTEGER;
    else if (name == "ADD") return requite::Opcode::ADD;
    else if (name == "SUBTRACT") return requite::Opcode::SUBTRACT;
    else if (name == "MULTIPLY") return requite::Opcode::MULTIPLY;
    else if (name == "DIVIDE") return requite::Opcode::DIVIDE;
    else if (name == "MODULO") return requite::Opcode::MODULO;
    else if (name == "COPY") return requite::Opcode::COPY;
    else if (name == "GREATER") return requite::Opcode::GREATER;
    else if (name == "LESS") return requite::Opcode::LESS;
    else if (name == "GREATER_EQUAL") return requite::Opcode::GREATER_EQUAL;
    else if (name == "GREATER_LESS") return requite::Opcode::GREATER_LESS;
    else if (name == "EQUAL") return requite::Opcode::EQUAL;
    else if (name == "NOT_EQUAL") return requite::Opcode::NOT_EQUAL;
    else if (name == "NOT") return requite::Opcode::NOT;
    else if (name == "AND") return requite::Opcode::AND;
    else if (name == "OR") return requite::Opcode::OR;
    else if (name == "IF") return requite::Opcode::IF;
    else if (name == "ELSE_IF") return requite::Opcode::ELSE_IF;
    else if (name == "ELSE") return requite::Opcode::ELSE;
    else if (name == "GOTO") return requite::Opcode::GOTO;
    else if (name == "LABEL") return requite::Opcode::LABEL;
    return requite::Opcode::UNKNOWN;
}

}