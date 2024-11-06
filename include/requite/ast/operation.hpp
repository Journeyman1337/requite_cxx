// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <requite/ast/opcode.hpp>

#include <vector>
#include <memory>

namespace requite {

struct Expression;

struct Operation final
{
    requite::Opcode opcode = requite::Opcode::UNKNOWN;
    std::unique_ptr<std::vector<std::unique_ptr<requite::Expression>>> expressions_uptr =
        std::make_unique<
            std::vector<
                std::unique_ptr<
                    requite::Expression
                >
            >
        >();
};

}