// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <cataloger/cataloger.hpp>
#include <operation.hpp>
#include <procedure.hpp>
#include <binary.hpp>

#include <cassert>

namespace r {

void Cataloger::tabulate_procedure(const r::Operation& operation, const r::Operation* attributes_operation)
{
    assert(r::get_is_procedure_opcode(operation.opcode));
    r::Procedure& procedure = this->resolver.add_procedure();
    procedure.module_symbol_i = this->module_symbol_i++;
    procedure.category = r::to_procedure_category(operation.opcode);
    procedure.declaration = &operation;
    procedure.module = this->resolver.module;
    procedure.object = this->resolver.object;
    procedure.export_group = this->resolver.export_group;
    if (attributes_operation != nullptr)
    {
        procedure.attributes = r::Attributes(*attributes_operation);
    }
    if (operation.opcode == r::Opcode::ENTRY_POINT)
    {
        if (this->resolver.binary->entry_point != nullptr)
        {
            throw std::runtime_error("must be a single entry point.");
        }
        this->resolver.binary->entry_point = &procedure;
        return;
    }
    if (operation.opcode == r::Opcode::CONSTRUCTOR)
    {
        procedure.object->constructor_group.add_overload(procedure, this->resolver);
        return;
    }
    if (operation.opcode == r::Opcode::DESTRUCTOR)
    {
        procedure.object->destructor = &procedure;
        return;
    }
    const r::Expression name_expression = operation.branches.at(0UZ);
    assert(std::holds_alternative<std::string_view>(name_expression));
    procedure.name = std::get<std::string_view>(name_expression);
    this->resolver.add_to_table(procedure);
}

void Cataloger::catalog(r::Procedure& procedure)
{
    this->resolver.enter(procedure);

    procedure.body_start_i = 0UZ;

    if (procedure.get_is_default())
    {
        return;
    }
    if (
        procedure.category != r::ProcedureCategory::CONSTRUCTOR &&
        procedure.category != r::ProcedureCategory::ENTRY_POINT &&
        procedure.category != r::ProcedureCategory::DESTRUCTOR
    )
    {
        procedure.body_start_i++;
    }
    this->catalog_user_mangled_name(procedure);
    this->catalog_calling_convention(procedure);
    this->catalog_variadic_arguments(procedure);
    this->catalog_return_type(procedure);
    this->catalog_arguments(procedure);
    this->check_valid(procedure);

    this->resolver.clear();
}

void Cataloger::catalog_calling_convention(r::Procedure& procedure)
{
    if (procedure.attributes.attribute_span.empty())
    {
        return;
    } 
    const r::Operation* calling_convention_operation = procedure.attributes.try_get_attribute(r::Opcode::CALL_CONVENTION);
    if (calling_convention_operation == nullptr)
    {
        return;
    }
    assert(calling_convention_operation->opcode == r::Opcode::CALL_CONVENTION);
    assert(calling_convention_operation->branches.size() == 1UZ);
    const r::Expression& convention_expression = calling_convention_operation->branches.front();
    assert(std::holds_alternative<std::string_view>(convention_expression));
    std::string_view convention_name = std::get<std::string_view>(convention_expression);
    procedure.calling_convention = r::to_calling_convention(convention_name);
}

void Cataloger::catalog_variadic_arguments(r::Procedure& procedure)
{
    if (procedure.attributes.attribute_span.empty())
    {
        return;
    }
    const r::Operation* variadic_arguments_attribute = procedure.attributes.try_get_attribute(r::Opcode::VARIADIC_ARGUMENTS);
    if (variadic_arguments_attribute == nullptr)
    {
        return;
    }
    assert(variadic_arguments_attribute->opcode == r::Opcode::VARIADIC_ARGUMENTS);
    assert(variadic_arguments_attribute->branches.empty());
    procedure.has_variadic_arguments = true;
}

void Cataloger::catalog_user_mangled_name(r::Procedure& procedure)
{
    if (procedure.category == r::ProcedureCategory::ENTRY_POINT)
    {
        procedure.mangled_name = "main";
        return;
    }
    const r::Operation* mangled_name_attribute = procedure.attributes.try_get_attribute(r::Opcode::MANGLED_NAME);
    if (mangled_name_attribute == nullptr)
    {
        if (procedure.category == r::ProcedureCategory::EXTERNAL_FUNCTION)
        {
            procedure.mangled_name = procedure.name;
        }
        return;
    }
    assert(mangled_name_attribute->opcode == r::Opcode::MANGLED_NAME);
    assert(mangled_name_attribute->branches.size() == 1UZ);
    const r::Expression& mangled_name_expression = mangled_name_attribute->branches.front();
    assert(std::holds_alternative<std::string_view>(mangled_name_expression));
    procedure.mangled_name = std::get<std::string_view>(mangled_name_expression);
}

void Cataloger::catalog_arguments(r::Procedure& procedure)
{
    if (procedure.declaration->branches.size() <= procedure.body_start_i)
    {
        return;
    }
    const r::Expression& expression = procedure.declaration->branches.at(procedure.body_start_i);
    if (!std::holds_alternative<r::Operation>(expression))
    {
        return;
    }
    const r::Operation& operation = std::get<r::Operation>(expression);
    if (operation.opcode != r::Opcode::ARGUMENTS)
    {
        return;
    }   
    procedure.body_start_i++;
    assert(operation.branches.size() % 2UZ == 0UZ);
    const std::size_t listed_argument_count = operation.branches.size() / 2UZ;
    procedure.arguments.reserve(listed_argument_count);
    for (std::size_t argument_i = 0UZ; argument_i < operation.branches.size();)
    {
        r::ProcedureArgument& argument = procedure.arguments.emplace_back();
        argument.type = this->resolver.resolve_type(operation.branches.at(argument_i++));
        assert(std::holds_alternative<std::string_view>(operation.branches.at(argument_i)));
        argument.name = std::get<std::string_view>(operation.branches.at(argument_i++));
    }
}

void Cataloger::catalog_return_type(r::Procedure& procedure)
{
    if (procedure.category == r::ProcedureCategory::ENTRY_POINT)
    {
        procedure.return_type = r::SIGNED_32_BIT_INTEGER_TYPE;
        return;
    }
    if (
        procedure.category == r::ProcedureCategory::CONSTRUCTOR ||
        procedure.category == r::ProcedureCategory::DESTRUCTOR    
    )
    {
        procedure.return_type = r::VOID_TYPE;
        return;
    }
    const r::Expression& expression = procedure.declaration->branches.at(procedure.body_start_i++);
    procedure.return_type = this->resolver.resolve_type(expression);
}

void Cataloger::check_valid(const r::Procedure& procedure)
{
    if (procedure.has_variadic_arguments && procedure.calling_convention != r::CallingConvention::C)
    {
        throw std::runtime_error("functions with variadic arguments must use C call convention.");
    }
}

}