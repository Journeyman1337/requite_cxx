// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <object.hpp>
#include <operation.hpp>
#include <procedure.hpp>
#include <global.hpp>
#include <export_group.hpp>
#include <module/module.hpp>
#include <symbol_table.hpp>
#include <property.hpp>
#include <global.hpp>
#include <export_group.hpp>
#include <utility.hpp>

#include <cassert>
#include <string_view>
#include <ranges>
#include <utility>

namespace r {

r::Property& Object::add_property()
{
    std::unique_ptr<r::Property>& property_ptr = this->properties.emplace_back();
    property_ptr = std::make_unique<r::Property>();
    property_ptr->property_i = this->properties.size() - 1UZ;
    return *property_ptr.get();
}

r::Property& Object::get_property(std::string_view name)
{
    if (!this->property_table.contains(name))
    {
        throw std::runtime_error("property with name not found.");
    }
    r::Property* property_ptr = this->property_table.at(name);
    assert(property_ptr != nullptr);
    return *property_ptr;
}

bool Object::get_has_destructor() const noexcept
{
    return this->destructor != nullptr;
}

bool Object::get_has_autodestruct_property() const noexcept
{
    for (const std::unique_ptr<r::Property>& property_ptr : this->properties)
    {
        assert(property_ptr.get() != nullptr);
        const r::Property& property = *property_ptr.get();
        if (!property.type.get_is_object())
        {
            continue;
        }
        if (property.get_is_no_autodestruct())
        {
            continue;
        }
        return true;
    }
    return false;
}

r::Procedure& Object::get_destructor()
{
    assert(this->destructor != nullptr);
    return *this->destructor;
}

void Object::add_to_table(r::Property& property)
{
    if (this->property_table.contains(property.name))
    {
        r::throw_disambiguous_symbol_name();
    }
    this->property_table[property.name] = &property;
}

}