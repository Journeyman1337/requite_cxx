// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <local.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::generate_address_of_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::ADDRESS_OF);
    assert(operation.branches.size() == 1UZ);
    const r::Expression& variable_expression = operation.branches.front();
    if (std::holds_alternative<std::string_view>(variable_expression))
    {
        std::string_view local_name = std::get<std::string_view>(variable_expression);
        r::Local& local = this->get_local(local_name);
        r::Type type = local.type;
        if (local.type.get_is_array())
        {
            return local.llvm_alloca;
        }
        type.add_pointer();
        this->resolver.check_type_assignable_to_type(type, expected_type);
        return local.llvm_alloca;
    }
    r::unreachable();
}

}