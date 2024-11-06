// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <local.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <module/module.hpp>
#include <procedure_argument.hpp>

namespace r {

bool Local::get_is_generated() const noexcept
{
    return this->llvm_alloca != nullptr;
}

 bool Local::get_is_no_autodestruct() const noexcept
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