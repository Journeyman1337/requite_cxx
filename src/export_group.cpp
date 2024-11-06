// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <export_group.hpp>
#include <procedure_group.hpp>

namespace r {

r::ProcedureGroup& ExportGroup::add_procedure_group()
{
    std::unique_ptr<r::ProcedureGroup>& procedure_group_ptr = this->procedure_groups.emplace_back();
    procedure_group_ptr = std::make_unique<r::ProcedureGroup>();
    return *procedure_group_ptr.get();
}

}