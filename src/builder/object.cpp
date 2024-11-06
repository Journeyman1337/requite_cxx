// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <module/module.hpp>
#include <object.hpp>
#include <property.hpp>
#include <utility.hpp>

#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include <cassert>
#include <memory>
#include <ranges>

namespace r {

void Builder::generate_prototype(r::Object& object)
{
    r::Resolver old_resolver = this->resolver;
    this->resolver.enter(object);
    llvm::SmallVector<llvm::Type*> llvm_property_types{};
    llvm_property_types.reserve(object.properties.size());
    for (std::unique_ptr<r::Property>& property_ptr : object.properties)
    {
        assert(property_ptr.get() != nullptr);
        r::Property& property = *property_ptr.get();
        if (property.type.get_is_object())
        {
            r::Object* property_object_ptr = std::get<r::Object*>(property.type.root);
            assert(property_object_ptr != nullptr);
            r::Object& property_object = *property_object_ptr;
            assert(property_object.module != nullptr);
            if (property_object.module == object.module)
            {
                if (property_object.module_symbol_i > object.module_symbol_i)
                {
                    throw std::runtime_error("property object must be defined higher in source file.");
                }
            }
            else if (!this->resolver.module->import_set.contains(property_object.module->mangled_name))
            {
                throw std::runtime_error("property object from other module must be imported.");
            }
        }
        llvm::Type*& llvm_type = llvm_property_types.emplace_back();
        llvm_type = this->resolver.get_llvm_type(property.type);        
    }
    object.llvm_struct_type =
        llvm::StructType::
            create(
                this->resolver.get_llvm_context(),
                llvm_property_types,
                object.name,
                object.is_packed
            );
    this->resolver = old_resolver;
}

void Builder::generate_property_initializers(r::Procedure& procedure)
{
    assert(procedure.get_is_constructor());
    assert(procedure.object != nullptr);
    r::Object& object = *procedure.object;
    llvm::Value* llvm_this = procedure.get_this();
    r::Type this_type = r::Type(procedure.object);
    llvm::Type* llvm_type = this->resolver.get_llvm_type(this_type);;
    for (std::unique_ptr<r::Property>& property_ptr : object.properties)
    {
        r::Property& property = *property_ptr.get();
        const r::Expression& value_expression =
            property.declaration->branches.at(
                property.value_i
            );
        if (std::holds_alternative<r::Operation>(value_expression))
        {
            const r::Operation& value_operation = std::get<r::Operation>(value_expression);
            if (value_operation.opcode == r::Opcode::INDETERMINATE_VALUE)
            {
                assert(value_operation.branches.empty());
                continue;
            }
        }
        llvm::Value* llvm_store =
            this->generate_property_location(
                object,
                llvm_this,
                property.name
            );
        if (property.type.get_is_llvm_store_type())
        {
            assert(property.type.get_is_object()); // TODO support other types.
            r::Object& object = property.type.get_object();
            this->generate_store_expression(
                value_expression,
                llvm_store,
                property.type
            );
        }
        else
        {
            if (property.type.get_is_null())
            {
                this->check_is_null_value_expression(value_expression);
            }
            llvm::Value* llvm_value =
                this->generate_value_expression(
                    value_expression,
                    property.type
                );
            this->generate_value_assignment(
                llvm_store,
                llvm_value,
                false
            );  
        }
    }
}

llvm::Value* Builder::generate_owner_location(const r::Expression& expression)
{
    if (std::holds_alternative<std::string_view>(expression))
    {
        std::string_view name = std::get<std::string_view>(expression);
        llvm::Value* llvm_this = this->resolver.procedure->get_this();
        assert(llvm_this != nullptr);
        return llvm_this;
    }
    else if (std::holds_alternative<r::Operation>(expression))
    {
        r::Resolver old_resolver = this->resolver;
        const r::Operation* operation_ptr = &std::get<r::Operation>(expression);
        if (operation_ptr->opcode == r::Opcode::ACCESS_TABLE)
        {
            const r::Expression& resolved_expression = this->resolver.resolve_table(*operation_ptr);
            assert(std::holds_alternative<r::Operation>(resolved_expression));
            operation_ptr = &std::get<r::Operation>(resolved_expression);
        }
        assert(operation_ptr->opcode == r::Opcode::ACCESS_MEMBER);
        llvm::Value* member_owner_location =
            this->generate_access_member_location(
                *operation_ptr,
                true
            );
        this->resolver = old_resolver;
        return member_owner_location;
    }
    r::unreachable();
}


llvm::Value* Builder::generate_property_location(r::Object& object, llvm::Value* llvm_object_location, std::string_view name)
{
    r::Property& property = object.get_property(name);
    llvm::Type* llvm_property_type = 
        object.llvm_struct_type->
            getElementType(
                property.property_i
            );
    llvm::ConstantInt* llvm_property_i =
        llvm::ConstantInt::get(
            llvm::Type::getInt64Ty(this->resolver.get_llvm_context()),
            property.property_i
        );
    llvm::Value* llvm_property_location =
        this->llvm_builder->
            CreateGEP(
                llvm_property_type,
                llvm_object_location,
                {llvm_property_i},
                "property_ptr",
                true
            );
    return llvm_property_location;
}

llvm::Value* Builder::generate_this_location(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::THIS);
    assert(operation.branches.empty());
    assert(this->resolver.procedure != nullptr);
    assert(this->resolver.procedure->get_is_instanced());
    llvm::Value* llvm_this = this->resolver.procedure->get_this();
    return llvm_this;
}

}