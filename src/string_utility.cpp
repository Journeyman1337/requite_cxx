// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <string_utility.hpp>
#include <utility.hpp>

#include <string>
#include <string_view>
#include <cassert>
#include <cstddef>
#include <utility>
#include <sstream>

namespace r {

std::string get_escaped_string(std::string_view text)
{
    assert(!text.empty());
    assert(text.front() == '\"');
    assert(text.back() == '\"');

    std::string result{};
    result.reserve(text.size() - 2UZ);

    const auto get_char =
        [&](std::size_t get_char_i) -> char
        {
            if (get_char_i >= text.size() - 1UZ)
            {
                return '\0';
            }
            return text[get_char_i];
        };

    for (std::size_t char_i = 1UZ; char_i < text.size() - 1UZ; char_i++)
    {
        const char cur_char = get_char(char_i);
        if (cur_char == '\\')
        {
            const char next_char = get_char(++char_i);
            switch (next_char)
            {
                case 'a':
                    result.push_back('\a');
                    break;
                case 'b':
                    result.push_back('\b');
                    break;
                case 'e':
                    result.push_back('\e');
                    break;
                case 'f':
                    result.push_back('\f');
                    break;
                case 'n':
                    result.push_back('\n');
                    break;
                case 'r':
                    result.push_back('\r');
                    break;
                case 't':
                    result.push_back('\t');
                    break;
                case 'v':
                    result.push_back('\v');
                    break;
                case '\\':
                    result.push_back('\\');
                    break;
                case '\'':
                    result.push_back('\'');
                    break;
                case '\"':
                    result.push_back('\"');
                    break;
                //case 'x':
                    // TODO hex
                //case 'u': 
                    // TODO unicode
                //case 'U':
                    // TODO hex unicode
                default:
                    r::unreachable();
            }
        }
        else
        {
            result.push_back(cur_char);
        }
    }
    return result;
}

std::uint64_t to_number(std::string_view text)
{
    assert(!text.empty());
    std::uint64_t number{};
    std::stringstream sstr;
    sstr << text;
    sstr >> number;
    return number;
}

}