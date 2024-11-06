// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <symbol_table.hpp>
#include <procedure_group.hpp>

#include <llvm/ADT/SmallVector.h>

#include <string_view>
#include <memory>

namespace r {

struct Operation;
struct Module;

struct ExportGroup final
{
    std::string_view name;
    r::SymbolTable table;
    llvm::SmallVector<std::unique_ptr<r::ProcedureGroup>> procedure_groups;

    r::ProcedureGroup& add_procedure_group();
};

}