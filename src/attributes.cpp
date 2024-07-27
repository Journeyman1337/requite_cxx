// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <attributes.hpp>

#include <cassert>

namespace r {

Attributes::Attributes(const r::Operation& attributes_operation) noexcept
{
    assert(attributes_operation.opcode == r::Opcode::ATTRIBUTES);
    if (attributes_operation.branches.size() > 1UZ)
    {
        this->attribute_span =
            std::span<const r::Expression>(
                attributes_operation.branches.data(),
                attributes_operation.branches.size() - 1UZ
            );
    }
}

const r::Operation* Attributes::try_get_attribute(r::Opcode attribute_opcode) const noexcept
{
    for (const r::Expression& attribute_expression : this->attribute_span)
    {
        if (std::holds_alternative<r::Operation>(attribute_expression))
        {
            const r::Operation& attribute_operation = std::get<r::Operation>(attribute_expression);
            assert(r::get_is_attribute_opcode(attribute_operation.opcode));
            if (attribute_opcode == attribute_operation.opcode)
            {
                return &attribute_operation;
            }
        }
    }
    return nullptr;
}

}