// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <module/module.hpp>

#include <cassert>
#include <format>

namespace r {

void Module::determine_name()
{
    if (this->ast.empty())
    {
        return;
    }
    const r::Operation& first_operation = this->ast.front();
    if (first_operation.opcode == r::Opcode::MODULE)
    {
        assert(first_operation.branches.size() == 1UZ);
        const r::Expression& name_expression = first_operation.branches.front();
        assert(std::holds_alternative<std::string_view>(name_expression));
        this->mangled_name = std::get<std::string_view>(name_expression);
        std::string_view invalid_prefix = "_____";
        if (this->mangled_name.size() < invalid_prefix.size())
        {
            return;
        }
        if (std::equal(invalid_prefix.begin(), invalid_prefix.end(), this->mangled_name.begin()))
        {
            throw std::runtime_error("user module name must not start with 5 underscores.");
        }
    }
    else
    {
        this->mangled_name = 
            std::format(
                "_____{}", // starts with 5 underscores
                this->path.stem().c_str()
            );
    }
}

}