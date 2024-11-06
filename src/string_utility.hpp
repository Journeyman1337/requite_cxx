// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <string_view>
#include <cstdint>

namespace r {

std::string get_escaped_string(std::string_view text);

std::uint64_t to_number(std::string_view text);

}