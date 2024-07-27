// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <module/module.hpp>
#include <symbol_table.hpp>
#include <export_group.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Support/TargetSelect.h>

#include <map>
#include <string_view>
#include <vector>
#include <cstddef>
#include <memory>

namespace r {

// use inheritance to ensure that llvm context is destroyed last.
struct BinaryBase
{
    std::unique_ptr<llvm::LLVMContext> llvm_context;
};

struct Binary final : r::BinaryBase
{
    r::Procedure* entry_point = nullptr;
    std::vector<r::Module> modules{};
    std::vector<r::Module*> ordered_modules{};
    std::vector<std::unique_ptr<r::ExportGroup>> export_groups{};
    r::SymbolTable table;

    std::string llvm_target_triple{};
    llvm::TargetOptions llvm_target_options{};
    const llvm::Target* llvm_target = nullptr;
    llvm::TargetMachine* llvm_target_machine = nullptr;
    std::unique_ptr<llvm::DataLayout> llvm_data_layout{};

    std::map<std::string_view, r::Module*> module_map{};

    r::Module& add_module();
    r::Module& get_module(std::string_view name);
    void map_modules();
    void check_no_circular_imports();
    void determine_module_order();
    void initialize_llvm_context();
    r::ExportGroup& add_export_group();
};

}