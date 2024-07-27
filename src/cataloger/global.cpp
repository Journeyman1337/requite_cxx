// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <operation.hpp>
#include <module/module.hpp>
#include <global.hpp>

#include <cassert>
#include <string_view>

namespace r {

void Cataloger::tabulate_global(const r::Operation& operation, const r::Operation* attributes_operation)
{
    assert(operation.opcode == r::Opcode::GLOBAL);
    r::Global& global = this->resolver.add_global();
    assert(!operation.branches.empty());
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    global.name = std::get<std::string_view>(name_expression);
    global.export_group = this->resolver.export_group;
    global.module_symbol_i = this->module_symbol_i++;
    global.object = this->resolver.object;
    global.module = this->resolver.module;
    global.declaration = &operation;
    if (attributes_operation != nullptr)
    {
        global.attributes = r::Attributes(*attributes_operation);
    }
    this->resolver.add_to_table(global);
}

}