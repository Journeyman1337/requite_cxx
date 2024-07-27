// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <vector>
#include <filesystem>

namespace r {

struct BuildCommand final
{
    std::vector<std::filesystem::path> source_files{};
};

}