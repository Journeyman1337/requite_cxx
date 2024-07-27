// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <module/module.hpp>
#include <procedure.hpp>
#include <procedure_group.hpp>
#include <global.hpp>
#include <object.hpp>
#include <binary.hpp>
#include <builder/builder.hpp>
#include <utility.hpp>

#include <memory>
#include <sstream>

namespace r {

r::Procedure& Module::add_procedure()
{
    std::unique_ptr<r::Procedure>& procedure_ptr = this->procedures.emplace_back();
    procedure_ptr = std::make_unique<r::Procedure>();
    return *procedure_ptr.get();
}

r::Global& Module::add_global()
{
    std::unique_ptr<r::Global>& global_ptr = this->globals.emplace_back();
    global_ptr = std::make_unique<r::Global>();
    return *global_ptr.get();
}

r::Object& Module::add_object()
{
    std::unique_ptr<r::Object>& object_ptr = this->objects.emplace_back();
    object_ptr = std::make_unique<r::Object>();
    return *object_ptr.get();
}

r::ObjectExtension& Module::add_object_extension()
{
    std::unique_ptr<r::ObjectExtension>& object_extension_ptr = this->object_extensions.emplace_back();
    object_extension_ptr = std::make_unique<r::ObjectExtension>();
    return *object_extension_ptr.get();
}

r::ProcedureGroup& Module::add_procedure_group()
{
    std::unique_ptr<r::ProcedureGroup>& procedure_group_ptr = this->procedure_groups.emplace_back();
    procedure_group_ptr = std::make_unique<r::ProcedureGroup>();
    return *procedure_group_ptr.get();
}

r::TypeAlias& Module::add_type_alias()
{
    std::unique_ptr<r::TypeAlias>& type_alias_ptr = this->type_aliases.emplace_back();
    type_alias_ptr = std::make_unique<r::TypeAlias>();
    return *type_alias_ptr.get();
}

}