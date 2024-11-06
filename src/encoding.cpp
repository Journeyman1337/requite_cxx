// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <encoding.hpp>
#include <utility.hpp>

#include <string_view>
#include <unordered_map>
#include <cstddef>

namespace r {

std::string_view to_string(r::Encoding encoding)
{
    switch (encoding) 
    {
        case r::Encoding::UNKNOWN:
            return "unknown";
        case r::Encoding::ASCII:
            return "ascii";
        case r::Encoding::CP1251:
            return "cp1251";
        case r::Encoding::CP1252:
            return "cp1252";
        case r::Encoding::CP437:
            return "cp437";
        case r::Encoding::LATIN1:
            return "latin1";
        case r::Encoding::UTF8:
            return "utf8";
        case r::Encoding::UTF16:
            return "utf16";
        case r::Encoding::UTF16LE:
            return "utf16le";
        case r::Encoding::UTF16BE:
            return "utf16be";
        case r::Encoding::UTF32:
            return "utf32";
        case r::Encoding::UTF32LE:
            return "utf32le";
        case r::Encoding::UTF32BE:
            return "utf32be";
    }
    return "unknown";
}

r::Encoding to_encoding(std::string_view text) {
    static const std::unordered_map<std::string_view, r::Encoding> map = 
        {
            {"unknown", r::Encoding::UNKNOWN},
            {"ascii", r::Encoding::ASCII},
            {"cp1251", r::Encoding::CP1251},
            {"cp1252", r::Encoding::CP1252},
            {"cp437", r::Encoding::CP437},
            {"latin1", r::Encoding::LATIN1},
            {"utf8", r::Encoding::UTF8},
            {"utf16", r::Encoding::UTF16},
            {"utf16le", r::Encoding::UTF16LE},
            {"utf16be", r::Encoding::UTF16BE},
            {"utf32", r::Encoding::UTF32},
            {"utf32le", r::Encoding::UTF32LE},
            {"utf32be", r::Encoding::UTF32BE}
        };
    auto it = map.find(text);
    if (it != map.end()) 
    {
        return it->second;
    }
    return r::Encoding::UNKNOWN;
}

std::size_t get_codeunit_bit_depth(r::Encoding encoding)
{
   switch (encoding) 
    {
        case r::Encoding::UNKNOWN:
            r::unreachable();
        case r::Encoding::ASCII:
            return 8UZ;
        case r::Encoding::CP1251:
            return 8UZ;
        case r::Encoding::CP1252:
            return 8UZ;
        case r::Encoding::CP437:
            return 8UZ;
        case r::Encoding::LATIN1:
            return 8UZ;
        case r::Encoding::UTF8:
            return 8UZ;
        case r::Encoding::UTF16:
            return 16UZ;
        case r::Encoding::UTF16LE:
            return 16UZ;
        case r::Encoding::UTF16BE:
            return 16UZ;
        case r::Encoding::UTF32:
            return 32UZ;
        case r::Encoding::UTF32LE:
            return 32UZ;
        case r::Encoding::UTF32BE:
            return 32UZ;
    }
    r::unreachable();
}

bool get_is_ascii_compatible(r::Encoding encoding)
{
    return
        encoding == r::Encoding::ASCII ||
        encoding == r::Encoding::LATIN1 ||
        encoding == r::Encoding::UTF8;
}

}