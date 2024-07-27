// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <operation.hpp>
#include <opcode.hpp>
#include <utility.hpp>

#include <cassert>

namespace r {

void Builder::attribute_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& type)
{
    assert(llvm_store != nullptr);
    assert(operation.opcode == r::Opcode::ATTRIBUTES);
}

void Builder::generate_store_expression(const r::Expression& expression, llvm::Value* llvm_store, const r::Type& expected_type)
{
    assert(llvm_store != nullptr);
    assert(llvm_store != nullptr);
    if (std::holds_alternative<r::Literal>(expression))
    {

    }
    else if (std::holds_alternative<std::string_view>(expression))
    {

    }
    else if (std::holds_alternative<r::Operation>(expression))
    {
        const r::Operation& operation = std::get<r::Operation>(expression);
        switch (operation.opcode)
        {
            case r::Opcode::CONSTRUCT:
                this->generate_construct_store_expression(operation, llvm_store, expected_type);
                break;
            case r::Opcode::CALL:
                this->generate_call_store_expression(operation, llvm_store, expected_type);
                break;
            default:
                r::unreachable();
        }
    }
    else
    {
        r::unreachable();
    }    
}

}