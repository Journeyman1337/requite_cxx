// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <resolver/resolver.hpp>
#include <module/module.hpp>
#include <export_group.hpp>
#include <binary.hpp>

#include <cassert>

namespace r {

r::ExportGroup& Resolver::add_export_group()
{
    assert(this->binary != nullptr);
    return this->binary->add_export_group();
}

r::Procedure& Resolver::add_procedure()
{
    assert(this->module != nullptr);
    return this->module->add_procedure();
}

r::Global& Resolver::add_global()
{
    assert(this->module != nullptr);
    return this->module->add_global();
}

r::Object& Resolver::add_object()
{
    assert(this->module != nullptr);
    return this->module->add_object();
}

r::ObjectExtension& Resolver::add_object_extension()
{
    assert(this->module != nullptr);
    return this->module->add_object_extension();
}

r::ProcedureGroup& Resolver::add_procedure_group()
{
    if (this->export_group != nullptr)
    {
        return this->export_group->add_procedure_group();
    }
    assert(this->module != nullptr);
    return this->module->add_procedure_group();
}

r::Property& Resolver::add_property()
{
    assert(this->object != nullptr);
    return this->object->add_property();
}

r::TypeAlias& Resolver::add_type_alias()
{
    assert(this->module != nullptr);
    return this->module->add_type_alias();
}

}