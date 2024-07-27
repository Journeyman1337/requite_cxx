// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <binary.hpp>

#include <cassert>

namespace r {

llvm::Value* Builder::generate_index_into_location(const r::Operation& operation)
{
    assert(operation.opcode == r::Opcode::INDEX_INTO);
    assert(operation.branches.size() == 2UZ);
    const r::Expression& variable_expression = operation.branches.front();
    const r::Expression& index_expression = operation.branches.back();
    r::Type index_type = this->resolver.deduce_type(index_expression, this);
    if (!index_type.get_is_integer())
    {
        throw std::runtime_error("index type must be integer.");
    }
    llvm::Value* llvm_index = 
        this->generate_value_expression(
            index_expression,
            index_type
        );
    llvm::Value* llvm_location =
        this->generate_store_location(
            variable_expression,
            false
        );
    r::Type type = this->resolver.deduce_type(variable_expression, this);
    assert(type.get_is_indexable());
    type.subtypes.pop_back();
    llvm::Type* llvm_element_type = this->resolver.get_llvm_type(type);
    llvm::Value* llvm_indexed_location =
        this->llvm_builder->
            CreateGEP(
                llvm_element_type,
                llvm_location,
                {llvm_index},
                "index_into"
            );
    return llvm_indexed_location;
}

llvm::Value* Builder::generate_index_into_value_expression(const r::Operation& operation, const r::Type& expected_type)
{
    assert(operation.opcode == r::Opcode::INDEX_INTO);
    assert(operation.branches.size() == 2UZ);  
    const r::Expression& branch_expression = operation.branches.front();
    r::Type deduced_type = this->resolver.deduce_type(branch_expression, this);
    this->resolver.check_indexed_type_assignable_to_type(deduced_type, expected_type);
    llvm::Value* llvm_index_into_location = this->generate_index_into_location(operation);
    llvm::Type* llvm_type = this->resolver.get_llvm_type(expected_type);
    return         
        this->llvm_builder->
            CreateLoad(
                llvm_type,
                llvm_index_into_location,
                "indexed_into"
            );
}

}