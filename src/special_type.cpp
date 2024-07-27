// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <special_type.hpp>
#include <utility.hpp>

#include <cstddef>

namespace r {

std::size_t get_bit_depth(r::SpecialType special_type)
{
    switch (special_type)
    {
        case r::SpecialType::BOOL:
            return 8UZ;
        case r::SpecialType::BYTE:
            return 8UZ;
        case r::SpecialType::VARIADIC_ARGUMENTS:
            return 24UZ;
        case r::SpecialType::VOID:
            return 0UZ;
    }
    r::unreachable();
}

}