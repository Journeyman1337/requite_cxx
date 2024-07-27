// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <expression.hpp>
#include <symbol.hpp>
#include <symbol_table.hpp>
#include <object.hpp>
#include <object_extension.hpp>
#include <procedure.hpp>
#include <global.hpp>
#include <type_alias.hpp>

#include "llvm/IR/Module.h"
#include <llvm/IR/Value.h>

#include <filesystem>
#include <string>
#include <vector>
#include <memory>
#include <set>

namespace r {

struct Binary;
struct Literal;
struct Object;
struct ObjectExtension;
struct Procedure;
struct Global;
struct ProcedureGroup;

// Tracks everything associated with a source code file.
struct Module final
{
    std::filesystem::path path{};
    std::string source{};
    std::string mangled_name{};
    std::size_t first_declaration_i = 0UZ;
    std::vector<r::Module*> import_vector{};
    std::set<std::string_view> import_set{};
    bool is_expanded = false;
    std::vector<r::Operation> ast{};
    r::Binary* binary = nullptr;
    std::vector<std::unique_ptr<r::Procedure>> procedures{};
    std::vector<std::unique_ptr<r::Global>> globals{};
    std::vector<std::unique_ptr<r::Object>> objects{};
    std::vector<std::unique_ptr<r::ObjectExtension>> object_extensions{};
    std::vector<std::unique_ptr<r::ProcedureGroup>> procedure_groups{};
    std::vector<std::unique_ptr<r::TypeAlias>> type_aliases{};
    r::SymbolTable table{};

    // the index of this module in Binary::ordered_modules;
    std::size_t module_i = 0UZ;
    // the index of the module before this module in Binary::ordered_modules
    // that must be completed before processing of this module can start.
    std::size_t last_blocking_module_i = 0UZ;

    std::unique_ptr<llvm::Module> llvm_module{};

    // source.cpp
    void read_source(const std::filesystem::path& path);

    // ast.cpp
    void parse_ast();
    void write_ast(const std::filesystem::path& path);

    // name.cpp
    void determine_name();

    // imports.cpp
    void determine_imports();
    void expand_imports();

    // ir.cpp
    void initialize_llvm_module();
    void generate_ir();
    void write_ir_file();

    // resolve_type_aliases.cpp
    void resolve_type_aliases();

    // compile.cpp
    void compile_intermediate_file();

    // symbols.cpp
    r::Procedure& add_procedure();
    r::Global& add_global();
    r::Object& add_object();
    r::ObjectExtension& add_object_extension();
    r::ProcedureGroup& add_procedure_group();
    r::TypeAlias& add_type_alias();
};

} // namespace r