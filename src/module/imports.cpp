// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <module/module.hpp>
#include <binary.hpp>

#include <cassert>
#include <format>
#include <ranges>

namespace r {

void Module::determine_imports()
{
    assert(this->binary != nullptr);
    if (this->ast.empty())
    {
        return;
    }
    const r::Operation* operation_ptr = &this->ast.front();
    if (operation_ptr->opcode == r::Opcode::MODULE)
    {
        if (this->ast.size() == 1UZ)
        {
            return;
        }
        operation_ptr = &this->ast.at(1UZ);
    }
    if (operation_ptr->opcode == r::Opcode::IMPORT)
    {
        this->import_vector.reserve(operation_ptr->branches.size());
        for (const r::Expression& import_expression : operation_ptr->branches)
        {
            assert(std::holds_alternative<std::string_view>(import_expression));
            std::string_view module_name = std::get<std::string_view>(import_expression);
            r::Module& module = this->binary->get_module(module_name);
            assert(!this->import_set.contains(module_name));
            this->import_vector.push_back(&module);
            this->import_set.emplace(module_name);
        }
    }
}

void Module::expand_imports()
{
    if (this->is_expanded)
    {
        return;
    }
    this->is_expanded = true;
    for (std::size_t import_i = 0UZ; import_i < this->import_vector.size(); import_i++)
    {
        r::Module& import = *this->import_vector.at(import_i);
        import.expand_imports();
        for (r::Module* import_import_ptr : import.import_vector)
        {
            if (!this->import_set.contains(import_import_ptr->mangled_name))
            {
                this->import_set.emplace(import_import_ptr->mangled_name);
                this->import_vector.push_back(import_import_ptr);
            }
        }
    }
}

}