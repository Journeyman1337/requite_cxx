// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <attributes.hpp>
#include <local.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <module/module.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

void Builder::generate_local_statement(const r::Operation& operation, const r::Operation* attributes_ptr)
{
    assert(operation.opcode == r::Opcode::LOCAL);
    r::Local* local_ptr = nullptr;
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    std::string_view name = std::get<std::string_view>(name_expression);
    if (operation.branches.size() == 3UZ)
    {
        const r::Expression& type_expression = operation.branches.at(1UZ);
        r::Type type = this->resolver.resolve_type(type_expression);
        type.resolve_type_alias();
        local_ptr = &this->add_local(name, type);
        local_ptr->has_value = true;
    }  
    else if (operation.branches.size() == 2UZ)
    {
        const r::Expression& last_expression = operation.branches.back();
        r::Type type = this->resolver.resolve_type(last_expression, true);
        const bool deduced = type.get_is_empty();
        if (deduced)
        {
            type = this->resolver.deduce_type(last_expression, this);
            
        }
        type.resolve_type_alias();
        local_ptr = &this->add_local(name, type);
        if (deduced)
        {
            local_ptr->has_value = true;
        }
    }
    else
    {
        r::unreachable();
    }
    assert(local_ptr != nullptr);
    if (attributes_ptr != nullptr)
    {
        local_ptr->attributes = r::Attributes(*attributes_ptr);
    }
    this->generate_local(*local_ptr);
    if (local_ptr->has_value)
    {
        if (local_ptr->type.get_is_llvm_value_type())
        {
            const r::Expression& value_expression = operation.branches.back();
            if (this->get_is_indeterminate_value(value_expression))
            {
                return;
            }
            if (local_ptr->type.get_is_null())
            {
                this->check_is_null_value_expression(value_expression);
            }
            llvm::Value* llvm_value =
                this->generate_value_expression(
                        value_expression,
                        local_ptr->type
                    );
            this->generate_value_assignment(
                local_ptr->llvm_alloca,
                llvm_value
            );
        }
        else if (local_ptr->type.get_is_llvm_store_type())
        {
            const r::Expression& store_expression = operation.branches.back();
            this->generate_store_expression(
                store_expression,
                local_ptr->llvm_alloca,
                local_ptr->type
            );
        }
        else
        {
            r::unreachable();
        }
    }
}

llvm::Value* Builder::generate_local_value_expression(const r::Operation& operation, const r::Operation* attributes_ptr, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::LOCAL);
    r::Local* local_ptr = nullptr;
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    std::string_view name = std::get<std::string_view>(name_expression);
    if (operation.branches.size() == 3UZ)
    {
        const r::Expression& type_expression = operation.branches.at(1UZ);
        r::Type type = this->resolver.resolve_type(type_expression);
        type.resolve_type_alias();
        local_ptr = &this->add_local(name, type);
        local_ptr->has_value = true;
    }  
    else if (operation.branches.size() == 2UZ)
    {
        const r::Expression& last_expression = operation.branches.back();
        r::Type type = this->resolver.resolve_type(last_expression, true);
        const bool deduced = type.get_is_empty();
        if (deduced)
        {
            type = this->resolver.deduce_type(last_expression, this);
            
        }
        type.resolve_type_alias();
        local_ptr = &this->add_local(name, type);
        if (deduced)
        {
            local_ptr->has_value = true;
        }
    }
    else
    {
        r::unreachable();
    }
    assert(local_ptr != nullptr);
    assert(!local_ptr->type.get_is_llvm_store_type());
    if (attributes_ptr != nullptr)
    {
        local_ptr->attributes = r::Attributes(*attributes_ptr);
    }
    this->generate_local(*local_ptr);
    llvm::Value* llvm_value = nullptr;
    if (local_ptr->has_value)
    {
        if (local_ptr->type.get_is_llvm_value_type())
        {
            const r::Expression& value_expression = operation.branches.back();
            if (this->get_is_indeterminate_value(value_expression))
            {
                llvm_value =
                    this->llvm_builder->
                        CreateLoad(
                            local_ptr->llvm_alloca->getAllocatedType(),
                            local_ptr->llvm_alloca,
                            name
                        );
                return llvm_value;
            }
            llvm_value =
                this->generate_value_expression(
                        value_expression,
                        local_ptr->type
                    );
            this->generate_value_assignment(
                local_ptr->llvm_alloca,
                llvm_value
            );
        }
        else
        {
            r::unreachable();
        }
    }
    return llvm_value;
}

void Builder::generate_local_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Operation* attributes_ptr, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::LOCAL);
    r::Local* local_ptr = nullptr;
    const r::Expression& name_expression = operation.branches.front();
    assert(std::holds_alternative<std::string_view>(name_expression));
    std::string_view name = std::get<std::string_view>(name_expression);
    if (operation.branches.size() == 3UZ)
    {
        const r::Expression& type_expression = operation.branches.at(1UZ);
        r::Type type = this->resolver.resolve_type(type_expression);
        type.resolve_type_alias();
        local_ptr = &this->add_local(name, type);
        local_ptr->has_value = true;
    }  
    else if (operation.branches.size() == 2UZ)
    {
        const r::Expression& last_expression = operation.branches.back();
        r::Type type = this->resolver.resolve_type(last_expression, true);
        const bool deduced = type.get_is_empty();
        if (deduced)
        {
            type = this->resolver.deduce_type(last_expression, this);
            
        }
        type.resolve_type_alias();
        local_ptr = &this->add_local(name, type);
        if (deduced)
        {
            local_ptr->has_value = true;
        }
    }
    else
    {
        r::unreachable();
    }
    assert(local_ptr != nullptr);
    assert(!local_ptr->type.get_is_llvm_store_type());
    if (attributes_ptr != nullptr)
    {
        local_ptr->attributes = r::Attributes(*attributes_ptr);
    }
    this->generate_local(*local_ptr);
    llvm::Value* llvm_value = nullptr;
    if (local_ptr->has_value)
    {
        if (local_ptr->type.get_is_llvm_store_type())
        {
            const r::Expression& value_expression = operation.branches.back();
            if (this->get_is_indeterminate_value(value_expression))
            {
                // TODO memcpy
            }
            llvm_value =
                this->generate_value_expression(
                        value_expression,
                        local_ptr->type
                    );
            this->generate_value_assignment(
                local_ptr->llvm_alloca,
                llvm_value
            );
            this->generate_value_assignment(
                llvm_store,
                llvm_value
            );
        }
        else
        {
            r::unreachable();
        }
    }
}

}