// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <file_io.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace r {

std::string read_file_text(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("file does not exist.");
    }
    std::ifstream ifile(path);
    std::stringstream sstream;
    sstream << ifile.rdbuf();
    ifile.close();
    return sstream.str();
}

void write_file_text(const std::filesystem::path& path, std::string_view text)
{
    std::ofstream ofile(path);
    if (!ofile.is_open())
    {
        throw std::runtime_error("failed to open file for writing.");
    }
    ofile << text;
    ofile.close();
}

} // namespace r