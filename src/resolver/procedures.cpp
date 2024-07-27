// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <resolver/resolver.hpp>
#include <symbol_table.hpp>
#include <symbol_table.hpp>
#include <procedure_group.hpp>
#include <utility.hpp>
#include <procedure.hpp>
#include <object.hpp>

#include <stdexcept>
#include <ranges>

namespace r {

r::ProcedureGroup& Resolver::get_procedure_group(const r::Expression& expression, r::Builder* builder)
{
    if (std::holds_alternative<std::string_view>(expression))
    {
        std::string_view name = std::get<std::string_view>(expression);
        r::ProcedureGroup* procedure_group = this->get_current_table().try_get_procedure_group(name);
        if (procedure_group == nullptr)
        {
            throw std::runtime_error("no procedure of name.");
        }
        return *procedure_group;
    }
    else if (std::holds_alternative<r::Operation>(expression))
    {
        const r::Operation& operation = std::get<r::Operation>(expression);
        if (operation.opcode == r::Opcode::ACCESS_TABLE)
        {
            const r::Expression& resolved_expression = this->resolve_table(operation);
            assert(std::holds_alternative<std::string_view>(resolved_expression));
            std::string_view name = std::get<std::string_view>(resolved_expression);
            r::ProcedureGroup* procedure_group = this->table->try_get_procedure_group(name);
            this->reset_table();
            if (procedure_group == nullptr)
            {
                throw std::runtime_error("no procedure of name.");
            }
            return *procedure_group;
        }
        else if (operation.opcode == r::Opcode::ACCESS_MEMBER)
        {
            assert(operation.branches.size() >= 2UZ);
            r::Object* object_ptr = nullptr;
            for (const r::Expression& expression : operation.branches | std::views::take(operation.branches.size() - 1UZ))
            {
                r::Type deduced_type = this->deduce_type(expression, builder);
                object_ptr = &deduced_type.get_object();
            }
            const r::Expression& last_expression = operation.branches.back();
            assert(std::holds_alternative<std::string_view>(last_expression));
            std::string_view name = std::get<std::string_view>(last_expression);
            r::ProcedureGroup* procedure_group_ptr = object_ptr->table.try_get_procedure_group(name);
            assert(procedure_group_ptr != nullptr);
            assert(r::get_is_instanced(procedure_group_ptr->category));
            return *procedure_group_ptr;
        }
    }
    r::unreachable();
}

r::Procedure& Resolver::get_call_procedure(const r::Operation& call_operation, r::Builder* builder)
{
    assert(!call_operation.branches.empty());
    const r::Expression& name_expression = call_operation.branches.front();
    r::ProcedureGroup& procedure_group = this->get_procedure_group(name_expression, builder);
    std::vector<r::Type> argument_types;
    argument_types.reserve(call_operation.branches.size() - 1UZ);
    for (const r::Expression& expression : call_operation.branches | std::views::drop(1UZ))
    {
        argument_types.push_back(
            this->deduce_type(expression, builder)
        );
    }     
    r::Procedure& procedure = this->get_best_overload(procedure_group, argument_types);   
    return procedure;
}

r::Procedure& Resolver::get_construct_procedure(const r::Operation& call_operation, const r::Type& expected_type, r::Builder* builder)
{
    assert(!call_operation.branches.empty());
    const r::Expression& type_expression = call_operation.branches.front();
    r::Type type = this->resolve_type(type_expression);
    this->check_type_assignable_to_type(type, expected_type);
    assert(type.get_is_object());
    r::Object& object = type.get_object();
    r::ProcedureGroup& procedure_group = object.constructor_group;
    std::vector<r::Type> argument_types;
    argument_types.reserve(call_operation.branches.size() - 1UZ);
    for (const r::Expression& expression : call_operation.branches | std::views::drop(1UZ))
    {
        argument_types.push_back(
            this->deduce_type(expression, builder)
        );
    }     
    r::Procedure& procedure = this->get_best_overload(procedure_group, argument_types);   
    return procedure;
}

r::Procedure& Resolver::get_best_overload(r::ProcedureGroup& procedure_group, std::span<const r::Type> arguments)
{
    assert(!procedure_group.overloads.empty());
    r::Procedure* chosen_overload = nullptr;
    for (r::Procedure* overload : procedure_group.overloads)
    {
        if (overload->has_variadic_arguments)
        {
            if (overload->arguments.size() > arguments.size())
            {
                continue;
            }
        }
        else
        {
            if (overload->arguments.size() != arguments.size())
            {
                continue;
            }
        }
        bool fail = false;
        for (std::size_t arg_i = 0UZ; arg_i < overload->arguments.size(); arg_i++)
        {
            const r::ProcedureArgument& overload_arg = overload->arguments.at(arg_i);
            const r::Type& arg_type = arguments[arg_i];
            if (!this->get_is_type_assignable_to_type(arg_type, overload_arg.type))
            {
                fail = true;
                break;
            }
        }
        if (fail)
        {
            continue;
        }
        if (chosen_overload != nullptr)
        {
            throw std::runtime_error("can not choose overload due to ambiguity.");
        }
        chosen_overload = overload;
    }
    if (chosen_overload == nullptr)
    {
        throw std::runtime_error("no function of name with matching arguments.");
    }
    return *chosen_overload;
}

void Resolver::check_overload_is_unique(r::ProcedureGroup& procedure_group, const r::Procedure& overload) const
{
    // TODO
}

}