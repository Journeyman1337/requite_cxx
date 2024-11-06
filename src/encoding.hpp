// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>
#include <cstddef>

namespace r {

enum class Encoding
{
    UNKNOWN,
    ASCII,
    CP1251,
    CP1252,
    CP437,
    LATIN1,
    UTF8,
    UTF16,
    UTF16LE,
    UTF16BE,
    UTF32,
    UTF32LE,
    UTF32BE
};

std::string_view to_string(r::Encoding encoding);

r::Encoding to_encoding(std::string_view text);

std::size_t get_codeunit_bit_depth(r::Encoding encoding);

bool get_is_ascii_compatible(r::Encoding encoding);

}