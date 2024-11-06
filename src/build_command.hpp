// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/SmallVector.h>

#include <filesystem>

namespace r {

struct BuildCommand final
{
    llvm::SmallVector<std::filesystem::path> source_files{};
};

}