// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <resolver/resolver.hpp>

#include <vector>

namespace r {

struct ExportGroup;
struct Global;
struct Object;
struct ObjectExtension;
struct Property;
struct Procedure;
struct TypeAlias;

// Catalogs all global symbols so they can be looked up easily in later stages of compilation.
struct Cataloger final
{
    r::Resolver resolver{};
    std::size_t module_symbol_i = 0UZ;

    // module.cpp
    void tabulate(r::Module& module);
    void tabulate_extensions(r::Module& module);
    void catalog(r::Module& module);
private:
    // catalog.cpp
    void tabulate(const r::Operation& operation);
    void tabulate_extensions(const r::Operation& operation);

    // attributes.cpp
    void tabulate_attributes(const r::Operation& operation);

    // export group.cpp
    void tabulate_export_group(const r::Operation& operation);

    // global.cpp    
    void tabulate_global(const r::Operation& operation, const r::Operation* attributes_operation = nullptr);
    void catalog(r::Global& export_group);

    // object_extension.cpp
    void tabulate_object_extension(const r::Operation& operation);
    void catalog(r::ObjectExtension& object_extension);

    // object.cpp
    void tabulate_object(const r::Operation& operation, const r::Operation* attributes_operation = nullptr);
    void catalog(r::Object& object);

    // property.cpp
    void tabulate_property(const r::Operation& operation, const r::Operation* attributes_operation = nullptr);
    void catalog(r::Property& property);

    // procedure.cpp
    void tabulate_procedure(const r::Operation& operation, const r::Operation* attributes_operation = nullptr);
    void catalog(r::Procedure& procedure);
    void catalog_calling_convention(r::Procedure& procedure);
    void catalog_variadic_arguments(r::Procedure& procedure);
    void catalog_user_mangled_name(r::Procedure& procedure);
    void catalog_arguments(r::Procedure& procedure);
    void catalog_return_type(r::Procedure& procedure);
    void check_valid(const r::Procedure& procedure);

    // type_alias.cpp
    void tabulate_type_alias(const r::Operation& operation);
    void catalog(r::TypeAlias& type_alias);
};

}