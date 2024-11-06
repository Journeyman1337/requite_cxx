// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <resolver/resolver.hpp>
#include <module/module.hpp>
#include <binary.hpp>
#include <export_group.hpp>
#include <procedure.hpp>
#include <type_alias.hpp>
#include <object.hpp>

#include <cassert>

namespace r {

void Resolver::enter(r::Binary& binary)
{
    this->binary = &binary;
    this->module = nullptr;
    this->export_group = nullptr;
    this->object = nullptr;
    this->procedure = nullptr;
    this->table = nullptr;
}
void Resolver::enter(r::Module& module)
{
    this->binary = module.binary;
    this->module = &module;
    this->export_group = nullptr;
    this->object = nullptr;
    this->procedure = nullptr;
    this->table = nullptr;
}


void Resolver::enter(r::ExportGroup& export_group, r::Module& module)
{
    this->binary = module.binary;
    this->module = &module;
    this->export_group = &export_group;
    this->object = nullptr;
    this->procedure = nullptr;
    this->table = nullptr;
}

void Resolver::enter(r::Object& object)
{
    this->binary = object.module->binary;
    this->module = object.module;
    this->export_group = object.export_group;
    this->object = &object;
    this->procedure = nullptr;
    this->table = nullptr;
}

void Resolver::enter(r::Procedure& procedure)
{
    this->binary = procedure.module->binary;
    this->module = procedure.module;
    this->export_group = procedure.export_group;
    this->object = procedure.object;
    this->procedure = &procedure;
    this->table = nullptr;
}

void Resolver::clear()
{
    this->binary = nullptr;
    this->module = nullptr;
    this->export_group = nullptr;
    this->object = nullptr;
    this->procedure = nullptr;
    this->table = nullptr;
}

void Resolver::enter(r::TypeAlias& type_alias)
{
    assert(type_alias.module != nullptr);
    if (type_alias.object != nullptr)
    {
        this->enter(*type_alias.object);
    }
    else if (type_alias.export_group != nullptr)
    {
        this->enter(*type_alias.export_group, *type_alias.module);
    }
    else
    {
        this->enter(*type_alias.module);
    }
}

}