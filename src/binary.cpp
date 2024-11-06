// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <binary.hpp>

#include <llvm/IR/Type.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/TargetParser/Host.h>

#include <string>
#include <string_view>
#include <memory>
#include <stdexcept>
#include <set>

namespace r {

r::Module& Binary::add_module()
{
    r::Module& module = this->modules.emplace_back();
    module.binary = this;
    return module;
}

r::Module& Binary::get_module(std::string_view name)
{
    if (!this->module_map.contains(name))
    {
        throw std::runtime_error("module not found with name.");
    }
    r::Module* module_ptr = this->module_map.at(name);
    assert(module_ptr != nullptr);
    return *module_ptr;
}

void Binary::map_modules()
{
    assert(this->module_map.empty());
    for (r::Module& module : this->modules)
    {
        if (this->module_map.contains(module.mangled_name))
        {
            throw std::runtime_error("duplicate module with same name.");
        }
        this->module_map[module.mangled_name] = &module;
    }
}

void Binary::check_no_circular_imports()
{
    for (r::Module& module : this->modules)
    {
        for (r::Module* import_ptr : module.import_vector)
        {
            if (import_ptr->import_set.contains(module.mangled_name))
            {
                throw std::runtime_error("circular module import.");
            }
        }
    }
}

void Binary::determine_module_order()
{
    this->ordered_modules.reserve(this->modules.size());
    std::set<std::string_view> ordered_module_set;
    for (r::Module& module : this->modules)
    {
        if (module.import_vector.empty())
        {
            this->ordered_modules.push_back(&module);
            ordered_module_set.emplace(module.mangled_name);
        }
    }
    while (this->ordered_modules.size() != this->modules.size())
    {
        for (r::Module& module : this->modules)
        {
            if (ordered_module_set.contains(module.mangled_name))
            {
                continue;
            }
            bool pass = true;
            for (r::Module* import : module.import_vector)
            {
                if (ordered_module_set.contains(import->mangled_name))
                {
                    module.last_blocking_module_i = 
                        std::max(
                            module.last_blocking_module_i,
                            import->module_i
                        );
                    continue;
                }
                pass = false;
                break;
            }
            if (pass)
            {
                ordered_module_set.emplace(module.mangled_name);
                module.module_i = this->ordered_modules.size();
                this->ordered_modules.push_back(&module);
            }
        }
    }
}

void Binary::initialize_llvm_context()
{
    this->llvm_target_triple = llvm::sys::getDefaultTargetTriple();
    this->llvm_context = std::make_unique<llvm::LLVMContext>();
    std::string error;
    this->llvm_target = 
        llvm::TargetRegistry::lookupTarget(
            this->llvm_target_triple,
            error
        );
    if (this->llvm_target == nullptr)
    {
        throw std::runtime_error(error);
    }
    this->llvm_target_machine =
        this->llvm_target->createTargetMachine(
            this->llvm_target_triple,
            "generic",
            "",
            this->llvm_target_options,
            llvm::Reloc::PIC_
        );
    this->llvm_data_layout = 
        std::make_unique<llvm::DataLayout>(
            this->llvm_target_machine->createDataLayout()
        );
}

r::ExportGroup& Binary::add_export_group()
{
    std::unique_ptr<r::ExportGroup>& export_group_ptr = this->export_groups.emplace_back();
    export_group_ptr = std::make_unique<r::ExportGroup>();
    return *export_group_ptr.get();
}

}