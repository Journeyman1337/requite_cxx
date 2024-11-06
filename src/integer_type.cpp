// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <integer_type.hpp>

#include <string_view>
#include <unordered_map>

namespace r {

std::string_view to_string(r::IntegerType integer_type)
{
    switch (integer_type) 
    {
        case r::IntegerType::UNKNOWN:
            return "unknown";
        case r::IntegerType::SIGNED:
            return "signed";
        case r::IntegerType::UNSIGNED:
            return "unsigned";
    }
    return "unknown";
}

r::IntegerType to_integer_type(std::string_view text)
{
    static const std::unordered_map<std::string_view, r::IntegerType> map = 
        {
            {"unknown", r::IntegerType::UNKNOWN},
            {"signed", r::IntegerType::SIGNED},
            {"unsigned", r::IntegerType::UNSIGNED}
        };
    auto it = map.find(text);
    if (it != map.end()) 
    {
        return it->second;
    }
    return r::IntegerType::UNKNOWN;
}

}