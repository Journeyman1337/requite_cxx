// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <local.hpp>
#include <object.hpp>
#include <procedure.hpp>

#include <cassert>

namespace r {

void Builder::generate_desruct_statement(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::DESTRUCT);
    assert(operation.branches.size() == 1UZ);
    const r::Expression& destruct_target = operation.branches.front();
    this->generate_destruct(destruct_target);
}

void Builder::generate_destruct(const r::Expression& expression)
{
    r::Type deduced_type = this->resolver.deduce_type(expression, this);
    r::Object& object = deduced_type.get_object();
    r::Procedure& destructor = object.get_destructor();
    llvm::Value* llvm_location =
        this->generate_store_location(
            expression,
            false
        );
    this->llvm_builder->
        CreateCall(
            destructor.llvm_function,
            {llvm_location},
            "destruct"
        );
}

void Builder::generate_destruct(r::Local& local)
{
    if (!local.type.get_is_object())
    {
        return;
    }
    r::Object& object = local.type.get_object();
    if (object.get_has_destructor())
    {
        r::Procedure& destructor = object.get_destructor();
        this->llvm_builder->
            CreateCall(
                destructor.llvm_function,
                {local.llvm_alloca},
                "destruct"
            );
    }
    this->generate_autodestruct_object(
        object,
        local.llvm_alloca
    );
}

void Builder::generate_autodestruct_object(r::Object& object, llvm::Value* llvm_object_location)
{
    for (std::unique_ptr<r::Property>& property_ptr : object.properties)
    {
        assert(property_ptr.get() != nullptr);
        r::Property& property = *property_ptr.get();
        if (!property.type.get_is_object())
        {
            continue;
        }
        r::Object& property_object = property.type.get_object();
        if (property_object.get_has_destructor())
        {
            r::Procedure& destructor = property_object.get_destructor();
            this->llvm_builder->
                CreateCall(
                    destructor.llvm_function,
                    {llvm_object_location},
                    "destruct"
                );
        }
        if (property_object.get_has_autodestruct_property())
        {
            llvm::Value* llvm_property_location =
                this->generate_property_location(
                    object,
                    llvm_object_location,
                    property.name
                );
            this->generate_autodestruct_object(
                property_object,
                llvm_property_location
            );
        }
    }
}

void Builder::generate_autodestruct_scope()
{
    assert(!this->scopes.empty());
    const llvm::SmallVector<std::size_t>& top_scope = this->scopes.back();
    for (std::size_t local_i : top_scope)
    {
        std::unique_ptr<r::Local>& local_ptr = this->locals[local_i];
        assert(local_ptr.get() != nullptr);
        r::Local& local = *local_ptr.get();
        if (local.get_is_no_autodestruct())
        {
            continue;
        }
        this->generate_destruct(local);
    }
}

void Builder::generate_autodestruct_frame()
{
    for (std::unique_ptr<r::Local>& local_ptr : this->locals)
    {
        assert(local_ptr.get() != nullptr);
        r::Local& local = *local_ptr.get();
        if (local.get_is_no_autodestruct())
        {
            continue;
        }
        this->generate_destruct(local);
    }
}

}