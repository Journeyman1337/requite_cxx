// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <procedure_group.hpp>
#include <procedure_argument.hpp>
#include <type.hpp>
#include <procedure.hpp>
#include <resolver/resolver.hpp>

#include <cassert>
#include <variant>
#include <cstddef>

namespace r {

void ProcedureGroup::add_overload(r::Procedure& procedure, r::Resolver& resolver)
{
    if (this->category == r::ProcedureCategory::UNKNOWN)
    {
        this->category = procedure.category;
    }
    else
    {
        assert(this->category == procedure.category);
    }
    if (this->get_is_empty())
    {
        this->name = procedure.name;
        this->return_type = procedure.return_type;
    }
    else
    {
        assert(procedure.name == this->name);
    }
    resolver.check_overload_is_unique(*this, procedure);
    this->overloads.push_back(&procedure);
}

bool ProcedureGroup::get_is_empty() const noexcept
{
    return this->name.empty() || this->overloads.empty() || this->return_type.get_is_empty();
}

}