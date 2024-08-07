// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <literal.hpp>
#include <type.hpp>
#include <binary.hpp>
#include <llvm_extensions.hpp>
#include <string_utility.hpp>
#include <utility.hpp>

#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>

#include <string>
#include <cassert>

namespace r {

llvm::Value* Builder::generate_primitive_literal(const r::Literal& literal, bool is_negative, const r::Type& type)
{
    assert(type.get_is_primitive());
    assert(!type.get_is_pointer());
    llvm::Type* llvm_type = this->resolver.get_llvm_type(type);
    if (type.get_is_integer())
    {
        if (
            literal.type == r::LiteralType::CODEUNIT ||
            literal.type == r::LiteralType::STRING ||  
            literal.type == r::LiteralType::NUMBER_WITH_DECIMAL  
        )
        {
            throw std::runtime_error("invalid literal type.");
        }
        const r::Integer& integer = std::get<r::Integer>(type.root);
        llvm::APInt llvm_int =
            llvm::APInt(
                integer.bit_depth,
                literal.text,
                10
            );
        if (is_negative)
        {
            llvm_int.flipAllBits();
            llvm_int += 1;
        }
        return
            llvm::ConstantInt::get(
                llvm_type,
                llvm_int
            );
    }
    else if (type.get_is_floating_point())
    {
        if (
            literal.type == r::LiteralType::CODEUNIT ||
            literal.type == r::LiteralType::STRING 
        )
        {
            throw std::runtime_error("invalid literal type.");
        }
        const r::FloatingPoint& floating_point = std::get<r::FloatingPoint>(type.root);
        llvm::APFloat llvm_float =
            llvm::APFloat(
                r::get_float_semantics(floating_point),
                literal.text
            );
        if (is_negative)
        {
            llvm_float.changeSign();
        }
        return
            llvm::ConstantFP::get(
                llvm_type,
                llvm_float
            );
    }
    r::unreachable();
}

llvm::ConstantInt* Builder::generate_integer_constant(const r::Expression& expression, const r::Type& type)
{
    assert(type.get_is_integer());
    const r::Expression* cur_expression = &expression;
    bool is_negative = false;
    while (std::holds_alternative<r::Operation>(*cur_expression))
    {
        const r::Operation& cur_operation = std::get<r::Operation>(*cur_expression);
        assert(cur_operation.branches.size() == 1UZ);
        assert(cur_operation.opcode == r::Opcode::MINUS || cur_operation.opcode == r::Opcode::PLUS);
        if (cur_operation.opcode == r::Opcode::MINUS)
        {
            is_negative = !is_negative;
        }
        cur_expression = &cur_operation.branches.front();
    }
    assert(std::holds_alternative<r::Literal>(*cur_expression));
    const r::Literal& literal = std::get<r::Literal>(*cur_expression);
    assert(literal.type == r::LiteralType::NUMBER);
    r::Integer integer = std::get<r::Integer>(type.root);
    llvm::IntegerType* llvm_integer_type = this->resolver.get_llvm_type(integer);
    llvm::APInt llvm_int =
        llvm::APInt(
            llvm_integer_type->getBitWidth(),
            literal.text,
            10
        );
    if (is_negative)
    {
        llvm_int.flipAllBits();
    }
    return
        llvm::ConstantInt::get(
            this->resolver.get_llvm_context(),
            llvm_int
        );
}

llvm::GlobalVariable* Builder::generate_global_string(const r::Literal& literal)
{ // TODO encodings
    if (literal.type != r::LiteralType::STRING)
    {
        throw std::runtime_error("invalid literal.");
    }
    assert(!literal.text.empty());
    assert(literal.text.front() == '\"');
    assert(literal.text.back() == '\"');
    std::string string = r::get_escaped_string(literal.text);
    llvm::GlobalVariable* llvm_value = 
        this->llvm_builder->
            CreateGlobalString(
                string.c_str()
            );
    llvm_value->setName("string_literal");
    return llvm_value;
}

}