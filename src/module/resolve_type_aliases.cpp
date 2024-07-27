// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <module/module.hpp>
#include <type_alias.hpp>
#include <procedure.hpp>
#include <global.hpp>
#include <object.hpp>
#include <property.hpp>

#include <cassert>

namespace r {

void Module::resolve_type_aliases()
{
    for (std::unique_ptr<r::Procedure>& procedure_ptr : this->procedures)
    {
        r::Procedure& procedure = *procedure_ptr.get();
        procedure.return_type.resolve_type_alias();
        for (r::ProcedureArgument& argument : procedure.arguments)
        {
            argument.type.resolve_type_alias();
        }
    }
    for (std::unique_ptr<r::Global>& global_ptr : this->globals)
    {
        r::Global& global = *global_ptr.get();
        global.type.resolve_type_alias();
    }
    for (std::unique_ptr<r::Object>& object_ptr : this->objects)
    {
        r::Object& object = *object_ptr.get();
        for (std::unique_ptr<r::Property>& property_ptr : object.properties)
        {
            r::Property& property = *property_ptr.get();
            property.type.resolve_type_alias();
        }
    }
    for (std::unique_ptr<r::ObjectExtension>& object_extension_ptr : this->object_extensions)
    {
        r::ObjectExtension& object_extension = *object_extension_ptr.get();
        // TODO
    }
}

}