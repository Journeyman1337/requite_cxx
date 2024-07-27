// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <property.hpp>

#include <cassert>

namespace r {

bool Property::get_is_no_autodestruct() const noexcept
{
    const r::Operation* attribute_ptr = this->attributes.try_get_attribute(r::Opcode::NO_AUTODESTRUCT);
    if (attribute_ptr != nullptr)
    {
        assert(attribute_ptr->branches.empty());
        return true;
    }
    return false;
}

}