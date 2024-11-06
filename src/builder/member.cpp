// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <object.hpp>

#include <ranges>

namespace r {

llvm::Value* Builder::generate_access_member_location(const r::Operation& operation, bool ignore_last)
{
    assert(operation.opcode == r::Opcode::ACCESS_MEMBER);
    assert(operation.branches.size() > 1UZ);
    const r::Expression& front_expression = operation.branches.front();
    r::Type type =
        this->resolver.deduce_type(
            front_expression,
            this
        );
    llvm::Value* llvm_location = 
        this->generate_store_location(
            front_expression,
            false
        );
    std::size_t take_count = operation.branches.size();
    if (ignore_last)
    {
        take_count--;
    }
    for (const r::Expression& branch_expression : operation.branches | std::views::take(ignore_last) | std::views::drop(1UZ))
    {
        while (type.get_is_pointer())
        {
            llvm::Type* llvm_type = this->resolver.get_llvm_type(type);
            type.subtypes.pop_back();
            llvm_location =
                this->llvm_builder->
                    CreateLoad(
                        llvm_type,
                        llvm_location,
                        "ptr_access"
                    );
        }
        r::Object& object = type.get_object();
        assert(std::holds_alternative<std::string_view>(branch_expression));
        std::string_view name = std::get<std::string_view>(branch_expression);
        llvm_location = 
            this->generate_property_location(
                object,
                llvm_location,
                name
            );
        r::Property& property = object.get_property(name);
        type = property.type;
    }
    return llvm_location;
}

llvm::Value* Builder::generate_access_member_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::ACCESS_MEMBER);
    r::Type deduced_type = this->resolver.deduce_type(operation, this);
    this->resolver.check_type_assignable_to_type(deduced_type, expected_type);
    llvm::Value* llvm_member_location =
         this->generate_access_member_location(
            operation
        );
    llvm::Type* llvm_type = this->resolver.get_llvm_type(expected_type);
    llvm::Value* llvm_value =
        this->llvm_builder->
            CreateLoad(
                llvm_type,
                llvm_member_location,
                "member"
            );
    return llvm_value;
}

}