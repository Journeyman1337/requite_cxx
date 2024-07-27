// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <compiler.hpp>
#include <build_command.hpp>
#include <binary.hpp>
#include <module/module.hpp>
#include <cataloger/cataloger.hpp>
#include <llvm_extensions.hpp>
#include <builder/builder.hpp>

#include <cstdlib>

namespace r {

void Compiler::build(const r::BuildCommand& build_command)
{
    r::Binary binary;
    binary.modules.reserve(build_command.source_files.size());
    for (const std::filesystem::path& source_file : build_command.source_files)
    {
        r::Module& module = binary.add_module();
        module.read_source(source_file);
    }
    for (r::Module& module : binary.modules)
    {
        module.parse_ast();
    }
    for (r::Module& module : binary.modules)
    {
        //module.validate_ast();
    }
    for (r::Module& module : binary.modules)
    {
        module.determine_name();
    }
    binary.map_modules();
    for (r::Module& module : binary.modules)
    {
        module.determine_imports();
    }
    for (r::Module& module : binary.modules)
    {
        module.expand_imports();
    }
    binary.check_no_circular_imports();
    binary.determine_module_order();
    r::Cataloger cataloger;
    for (r::Module& module : binary.modules)
    {
        cataloger.tabulate(module);
    }
    for (r::Module& module : binary.modules)
    {
        cataloger.tabulate_extensions(module);
    }
    for (r::Module& module : binary.modules)
    {
        cataloger.catalog(module);
    }
    for (r::Module& module : binary.modules)
    {
        module.resolve_type_aliases();
    }
    //binary.generate_mangled_symbol_names();
    r::initialize_llvm();
    binary.initialize_llvm_context();
    for (r::Module& module : binary.modules)
    {
        module.initialize_llvm_module();
    }
    r::Builder builder;
    builder.initialize(binary);
    for (r::Module* module_ptr : binary.ordered_modules)
    {
        assert(module_ptr != nullptr);
        builder.generate_prototypes(*module_ptr);
    }
    for (r::Module& module : binary.modules)
    {
        builder.generate_module(module);
    }
    for (r::Module& module : binary.modules)
    {
        module.write_ir_file();
    }
    for (r::Module& module : binary.modules)
    {
        module.compile_intermediate_file();
    }
    //std::system("clang");
}

}