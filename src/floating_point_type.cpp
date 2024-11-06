// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <floating_point_type.hpp>
#include <utility.hpp>

#include <cstddef>
#include <string_view>
#include <unordered_map>

namespace r {

std::string_view to_string(r::FloatingPointType floating_point_type)
{
    switch (floating_point_type) 
    {
        case r::FloatingPointType::UNKNOWN:
            return "unknown";
        case r::FloatingPointType::BRAIN:
            return "brain";
        case r::FloatingPointType::BINARY_HALF:
            return "binary_half";
        case r::FloatingPointType::BINARY_SINGLE:
            return "binary_single";
        case r::FloatingPointType::BINARY_DOUBLE:
            return "binary_double";
        case r::FloatingPointType::BINARY_QUAD:
            return "binary_quad";
        case r::FloatingPointType::PCC_DOUBLE_DOUBLE:
            return "pcc_double_double";
        case r::FloatingPointType::FLOAT8_E5M2:
            return "float8_e6m2";
        case r::FloatingPointType::FLOAT8_E6M2FNUZ:
            return "float8_e6m2fnuz";
        case r::FloatingPointType::FLOAT8_E4M3FN:
            return "float8_e4m3fn";
        case r::FloatingPointType::FLOAT8_E4M3FNUZ:
            return "float8_e4m3fnuz";
        case r::FloatingPointType::FLOAT8_E4M3B11FNUZ:
            return "float8_4m3b11fnuz";
        case r::FloatingPointType::FLOAT_TF32:
            return "float_tf32";
        case r::FloatingPointType::X87_DOUBLE_EXTENDED:
            return "x87_double_extended";
    }
    return "unknown";
}

r::FloatingPointType to_floating_point_type(std::string_view text)
{
    static const std::unordered_map<std::string_view, r::FloatingPointType> map = 
        {
            {"unknown", r::FloatingPointType::UNKNOWN},
            {"brain", r::FloatingPointType::BRAIN},
            {"binary_half", r::FloatingPointType::BINARY_HALF},
            {"binary_single", r::FloatingPointType::BINARY_SINGLE},
            {"binary_double", r::FloatingPointType::BINARY_DOUBLE},
            {"binary_quad", r::FloatingPointType::BINARY_QUAD},
            {"pcc_double_double", r::FloatingPointType::PCC_DOUBLE_DOUBLE},
            {"float8_e6m2", r::FloatingPointType::FLOAT8_E5M2},
            {"float8_e6m2fnuz", r::FloatingPointType::FLOAT8_E6M2FNUZ},
            {"float8_e4m3fn", r::FloatingPointType::FLOAT8_E4M3FN},
            {"float8_e4m3fnuz", r::FloatingPointType::FLOAT8_E4M3FNUZ},
            {"float8_4m3b11fnuz", r::FloatingPointType::FLOAT8_E4M3B11FNUZ},
            {"float_tf32", r::FloatingPointType::FLOAT_TF32},
            {"x87_double_extended", r::FloatingPointType::X87_DOUBLE_EXTENDED}
        };
    auto it = map.find(text);
    if (it != map.end()) 
    {
        return it->second;
    }
    return r::FloatingPointType::UNKNOWN;
}

std::size_t get_bit_depth(r::FloatingPointType floating_point_type)
{
    switch (floating_point_type) 
    {
        case r::FloatingPointType::UNKNOWN:
            r::unreachable();
        case r::FloatingPointType::BRAIN:
            return 16UZ;
        case r::FloatingPointType::BINARY_HALF:
            return 16UZ;
        case r::FloatingPointType::BINARY_SINGLE:
            return 32UZ;
        case r::FloatingPointType::BINARY_DOUBLE:
            return 64UZ;
        case r::FloatingPointType::BINARY_QUAD:
            return 128UZ;
        case r::FloatingPointType::PCC_DOUBLE_DOUBLE:
            return 128UZ; // actually 106
        case r::FloatingPointType::FLOAT8_E5M2:
            return 8UZ;
        case r::FloatingPointType::FLOAT8_E6M2FNUZ:
            return 8UZ;
        case r::FloatingPointType::FLOAT8_E4M3FN:
            return 8UZ;
        case r::FloatingPointType::FLOAT8_E4M3FNUZ:
            return 8UZ;
        case r::FloatingPointType::FLOAT8_E4M3B11FNUZ:
            return 8UZ;
        case r::FloatingPointType::FLOAT_TF32:
            return 32UZ;
        case r::FloatingPointType::X87_DOUBLE_EXTENDED:
            return 128UZ; // actually 87
    }
    r::unreachable();
}

}