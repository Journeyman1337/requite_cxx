// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <operation.hpp>
#include <expression.hpp>

#include <span>

namespace r {

struct Attributes final
{
    std::span<const r::Expression> attribute_span{};

    Attributes() noexcept = default;
    Attributes(const r::Operation& attributes_operation) noexcept;

    const r::Operation* try_get_attribute(r::Opcode attribute_opcode) const noexcept;
};

}