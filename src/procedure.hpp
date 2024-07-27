// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <procedure_argument.hpp>
#include <procedure_category.hpp>
#include <attributes.hpp>
#include <type.hpp>
#include <calling_convention.hpp>

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>

#include <cstddef>
#include <string_view>
#include <span>
#include <string>

namespace r {

struct Operation;
struct Object;
struct ExportGroup;
struct Module;
struct Binary;
struct ProcedureGroup;

struct Procedure final
{
    std::string_view name{};
    std::string mangled_name{};
    std::size_t module_symbol_i = 0UZ;
    r::ProcedureCategory category = r::ProcedureCategory::UNKNOWN;
    r::Type return_type{};
    bool has_variadic_arguments = false;
    std::vector<r::ProcedureArgument> arguments{};
    // the index of the operation branch where the function body starts.
    std::size_t body_start_i = 0UZ;
    const r::Operation* declaration = nullptr;
    r::Object* object = nullptr;
    r::Module* module = nullptr;
    r::ExportGroup* export_group = nullptr;
    r::ProcedureGroup* procedure_group = nullptr;
    r::Attributes attributes;
    r::CallingConvention calling_convention = r::CallingConvention::C;

    // NOTE can be no pointer to any associated r::ProcedureGroup because it is stored in std::map.
    // not needed, so whatever.

    llvm::FunctionType* llvm_type = nullptr;
    llvm::Function* llvm_function = nullptr;

    bool get_has_body() const noexcept;
    bool get_is_instanced() const noexcept;
    bool get_has_sret() const noexcept;
    bool get_is_constructor() const noexcept;
    bool get_is_destructor() const noexcept;
    bool get_is_default() const noexcept;

    llvm::Value* get_this();
    llvm::Value* get_sret();

    r::Type get_sret_type() const noexcept;
    r::Type get_this_type() const noexcept;
};

}