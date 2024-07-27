// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <module/module.hpp>
#include <file_io.hpp>

#include <cassert>

namespace r {

void Module::read_source(const std::filesystem::path& path)
{
    assert(this->source.empty());
    assert(this->path.empty());
    this->path = path;
    this->source = r::read_file_text(path);
}

}