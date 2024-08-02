// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

bool Builder::get_is_indeterminate_value(const r::Expression& expression)
{
    if (!std::holds_alternative<r::Operation>(expression))
    {
        return false;
    }
    const r::Operation& operation = std::get<r::Operation>(expression);
    if (operation.opcode == r::Opcode::INDETERMINATE_VALUE)
    {
        assert(operation.branches.empty());
        return true;
    }
    return false;
}

llvm::Value* Builder::generate_store_location(const r::Expression& expression, bool is_assigning)
{
    if (std::holds_alternative<std::string_view>(expression))
    {
        std::string_view name = std::get<std::string_view>(expression);
        r::Local& local = this->get_local(name);
        if (is_assigning)
        {
            local.has_value = true;
        }
        else if (!local.has_value)
        {
            throw std::runtime_error("variable not assigned.");
        }
        return local.llvm_alloca;
    }
    else if (std::holds_alternative<r::Operation>(expression))
    {
        const r::Operation& operation = std::get<r::Operation>(expression);
        switch (operation.opcode)
        {
            case r::Opcode::DEREFERENCE:
                return this->generate_dereference_location(operation);
            case r::Opcode::INDEX_INTO:
                return this->generate_index_into_location(operation);
            case r::Opcode::ACCESS_MEMBER:
                return this->generate_access_member_location(operation);
            case r::Opcode::THIS:
                return this->generate_this_location(operation);
            case r::Opcode::CONSTRUCT:
                return this->generate_construct_temporary_location(operation);
            default:
                assert(false);
        }
    }
    assert(false);
    return nullptr;
}

void Builder::generate_value_assignment(llvm::Value* llvm_location, llvm::Value* llvm_value, bool is_volatile)
{
    this->llvm_builder->
        CreateStore(
            llvm_value,
            llvm_location,
            is_volatile
        );
}

void Builder::generate_assignment_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::EQUAL);
    assert(operation.branches.size() == 2UZ);
    const r::Expression& store_location_expression = operation.branches.front();
    r::Type expected_type = this->resolver.deduce_type(store_location_expression, this);
    const r::Expression& lhs_expression = operation.branches.back();
    llvm::Value* llvm_value =
        this->generate_value_expression(
            lhs_expression,
            expected_type
        );
    llvm::Value* llvm_local_store =
        this->generate_store_location(
            store_location_expression,
            true
        );
    this->generate_value_assignment(
        llvm_local_store,
        llvm_value
    );
}

llvm::Value* Builder::generate_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::EQUAL);
    assert(operation.branches.size() == 2UZ);
    const r::Expression& store_location_expression = operation.branches.front();
    r::Type deduced_thpe = this->resolver.deduce_type(store_location_expression, this);
    this->resolver.check_type_assignable_to_type(deduced_thpe, expected_type);
    const r::Expression& lhs_expression = operation.branches.back();
    llvm::Value* llvm_value =
        this->generate_value_expression(
            lhs_expression,
            expected_type
        );
    llvm::Value* llvm_local_store =
        this->generate_store_location(
            store_location_expression,
            true
        );
    this->generate_value_assignment(
        llvm_local_store,
        llvm_value
    );
    return llvm_value;
}

void Builder::generate_assignment_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::EQUAL);
    assert(operation.branches.size() == 2UZ);
    const r::Expression& store_location_expression = operation.branches.front();
    r::Type deduced_thpe = this->resolver.deduce_type(store_location_expression, this);
    this->resolver.check_type_assignable_to_type(deduced_thpe, expected_type);
    const r::Expression& lhs_expression = operation.branches.back();
    llvm::Value* llvm_value =
        this->generate_value_expression(
            lhs_expression,
            expected_type
        );
    llvm::Value* llvm_local_store =
        this->generate_store_location(
            store_location_expression,
            true
        );
    this->generate_value_assignment(
        llvm_local_store,
        llvm_value
    );
    this->generate_value_assignment(
        llvm_store,
        llvm_value
    );
}

}