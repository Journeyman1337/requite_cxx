// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <compiler.hpp>
#include <build_command.hpp>

int main()
{
    r::Compiler compiler;
    r::BuildCommand build_command;
    build_command.source_files =
        {
            "libc_stdio.requite",
            "libc_math.requite",
            "r_primitives.requite",
            "test.requite"
        };
    compiler.build(build_command);
    return 0;
}