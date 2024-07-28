// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <module/module.hpp>
#include <utility.hpp>

#include <ranges>
#include <cstddef>

namespace r {

void Cataloger::tabulate(r::Module& module)
{
    if (module.ast.empty())
    {
        return;
    }
    this->module_symbol_i = 0UZ;
    this->resolver.enter(module);
    assert(module.first_declaration_i == 0UZ);
    if (module.ast.front().opcode == r::Opcode::MODULE)
    {
        module.first_declaration_i++;
    }
    if (module.ast[module.first_declaration_i].opcode == r::Opcode::IMPORT)
    {
        module.first_declaration_i++;
    }
    for (const r::Operation& operation : module.ast | std::views::drop(module.first_declaration_i))
    {
        this->tabulate(operation);
    }
    this->resolver.clear();
}

void Cataloger::tabulate_extensions(r::Module& module)
{
    if (module.ast.empty())
    {
        return;
    }
    this->resolver.enter(module);
    for (const r::Operation& operation : module.ast | std::views::drop(module.first_declaration_i))
    {
        this->tabulate_extensions(operation);
    }
    this->resolver.clear();
}

void Cataloger::catalog(r::Module& module)
{
    if (module.ast.empty())
    {
        return;
    }
    for (std::unique_ptr<r::Procedure>& procedure_ptr : module.procedures)
    {
        r::Procedure& procedure = *procedure_ptr.get();
        this->catalog(procedure);
    }
    for (std::unique_ptr<r::Global>& global_ptr : module.globals)
    {
        r::Global& global = *global_ptr.get();
    }
    for (std::unique_ptr<r::Object>& object_ptr : module.objects)
    {
        r::Object& object = *object_ptr.get();
        this->catalog(object);
    }
    for (std::unique_ptr<r::ObjectExtension>& object_extension_ptr : module.object_extensions)
    {
        r::ObjectExtension& object_extension = *object_extension_ptr.get();
    }
    for (std::unique_ptr<r::TypeAlias>& type_alias_ptr : module.type_aliases)
    {
        r::TypeAlias& type_alias = *type_alias_ptr.get();
        this->catalog(type_alias);
    }
}

}