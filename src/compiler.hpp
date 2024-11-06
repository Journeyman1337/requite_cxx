// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <build_command.hpp>

namespace r {

struct Compiler final
{
    void build(const r::BuildCommand& build_command);
};

}