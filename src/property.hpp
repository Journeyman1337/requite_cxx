// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type.hpp>
#include <attributes.hpp>

#include <string_view>
#include <cstddef>

namespace r {

struct Object;
struct Operation;
struct Builder;

struct Property final
{
    std::string_view name;
    std::size_t property_i = 0UZ;
    std::size_t module_symbol_i = 0UZ;
    std::size_t value_i = 0UZ;
    r::Object* object = nullptr;
    r::Type type{};
    const r::Operation* declaration = nullptr;
    r::Attributes attributes{};

    bool get_is_no_autodestruct() const noexcept;
};

}