<!--
SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>

SPDX-License-Identifier: MIT
-->

# Requite

__NOTICE: This repository contains a compiler front-end that is in the early stages of development. Its buggy, incomplete, and hard to work with. Use it at your own risk!__

Requite is a statically typed systems programming langauge with a homoiconic syntax.

# Stages

1. Read source files
2. Parse abstract syntax tree for each file
3. Catalogue types and functions (so there is no need for forward declarations later)
4. Build ir with LLVM
5. Output intermediate files with LLVM
6. Compile with clang

## Syntax Examples

Example files can be found in the test_sources folder. There are no command line options yet, and all configuration should be done by changing the C++ source code in <src/main.cpp>. When running the test source files, make sure that there is only one `entry_point` operation across all source files. Some files contain definitions used by other sources, so you need to compile them together. For further instructions, look at the <test_sources/test.bash> file.