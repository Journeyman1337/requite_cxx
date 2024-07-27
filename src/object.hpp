// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <symbol_table.hpp>
#include <object_extension.hpp>
#include <attributes.hpp>
#include <property.hpp>
#include <procedure_group.hpp>

#include <llvm/IR/DerivedTypes.h>

#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>

namespace r {

struct Function;
struct ObjectExtension;
struct Operation;
struct ExportGroup;
struct SymbolTable;

struct Object final
{
    std::string_view name{};
    std::size_t module_symbol_i = 0UZ;
    bool is_packed = false;
    r::ExportGroup* export_group = nullptr;
    r::ProcedureGroup constructor_group{};
    r::Procedure* destructor = nullptr;
    r::SymbolTable table{};
    const r::Operation* declaration = nullptr;
    r::Module* module = nullptr;
    std::vector<std::unique_ptr<r::Property>> properties{};
    std::unordered_map<std::string_view, r::Property*> property_table{};
    r::Attributes attributes{};

    llvm::StructType* llvm_struct_type = nullptr;

    r::Property& add_property();
    r::Property& get_property(std::string_view name);
    bool get_has_destructor() const noexcept;
    bool get_has_autodestruct_property() const noexcept;
    r::Procedure& get_destructor();

    void add_to_table(r::Property& property);
};

}