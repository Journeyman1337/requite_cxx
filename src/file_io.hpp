// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <filesystem>
#include <string_view>

namespace r {

std::string read_file_text(const std::filesystem::path& path);

void write_file_text(const std::filesystem::path& path, std::string_view text);

} // namespace r