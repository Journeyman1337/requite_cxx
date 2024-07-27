# SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
#
# SPDX-License-Identifier: MIT

target_sources(
    requite
    PRIVATE
        "attributes.cpp"
        "binary.cpp"
        "build.cpp"
        "calling_convention.cpp"
        "encoding.cpp"
        "export_group.cpp"
        "file_io.cpp"
        "floating_point_type.cpp"
        "integer_type.cpp"
        "literal.cpp"
        "llvm_extensions.cpp"
        "local.cpp"
        "main.cpp"
        "object.cpp"
        "opcode.cpp"
        "procedure_category.cpp"
        "procedure_group.cpp"
        "procedure.cpp"
        "property.cpp"
        "special_type.cpp"
        "string_utility.cpp"
        "symbol_table.cpp"
        "type.cpp"
)

target_include_directories(
    requite
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_subdirectory(builder)
add_subdirectory(cataloger)
add_subdirectory(module)
add_subdirectory(resolver)