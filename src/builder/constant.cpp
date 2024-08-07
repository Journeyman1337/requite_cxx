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

llvm::ConstantInt* Builder::generate_integer_constant(llvm::APSInt llvm_ap_sint, const r::Type& type)
{
    assert(type.get_is_integer());
    r::Integer integer = type.get_integer();
    if (
        integer.bit_depth != llvm_ap_sint.getBitWidth()
    )
    {
        throw std::runtime_error("invalid constant integer bit depth.");
    }
    if (
        type.get_is_unsigned_integer() &&
        llvm_ap_sint.isSigned()
    )
    {
        throw std::runtime_error("invalid constant integer signedness.");
    }
    return
        llvm::ConstantInt::get(
            this->resolver.get_llvm_context(),
            llvm_ap_sint
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