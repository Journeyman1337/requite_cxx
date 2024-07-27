# SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
#
# SPDX-License-Identifier: MIT

target_sources(
    requite
    PRIVATE
        "ast.cpp"
        "compile.cpp"
        "imports.cpp"
        "ir.cpp"
        "name.cpp"
        "resolve_type_aliases.cpp"
        "source.cpp"
        "symbols.cpp"
)