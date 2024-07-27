// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type_root.hpp>
#include <expression.hpp>
#include <operation.hpp>
#include <qualifiers.hpp>
#include <subtype.hpp>

#include <llvm/IR/Type.h>

#include <variant>
#include <bitset>
#include <cstddef>

namespace r {

struct Module;
struct Literal;

struct Type final
{
    r::TypeRoot root{};
    r::QualifierFlagSet qualifiers{};
    std::vector<r::Subtype> subtypes{};

    Type() noexcept = default;
    constexpr Type(r::TypeRoot root) 
        : root(root)
    {}
    Type(const r::Literal& literal);

    void add_pointer();
    bool get_is_empty() const noexcept;
    void clear() noexcept;
    bool get_is_primitive() const noexcept;
    bool get_is_special_type(r::SpecialType special_type) const noexcept;
    bool get_is_pointer_to_codeunit() const noexcept;
    bool get_is_byte() const noexcept;
    bool get_is_numeric_primitive() const noexcept;
    bool get_is_integer() const noexcept;
    bool get_is_signed_integer() const noexcept;
    bool get_is_unsigned_integer() const noexcept;
    bool get_is_floating_point() const noexcept;
    bool get_is_fixed_point() const noexcept;
    bool get_is_bool() const noexcept;
    bool get_is_void() const noexcept;
    bool get_is_pointer() const noexcept;
    bool get_is_array() const noexcept;
    std::size_t get_array_size() const noexcept;
    bool get_is_indexable() const noexcept;
    bool get_is_endemic() const noexcept;
    bool get_is_object() const noexcept;
    bool get_is_mutable() const noexcept;
    bool get_is_volatile() const noexcept;
    void set_mutable(bool mutability) noexcept;
    bool get_is_literal() const noexcept;
    void clear_literals() noexcept;
    bool get_is_type_alias() const noexcept;
    void resolve_type_alias() noexcept;

    r::Object& get_object() noexcept;

    bool get_is_llvm_value_type() const noexcept;
    bool get_is_llvm_store_type() const noexcept;
};

constexpr r::Type BOOL_TYPE = r::Type(r::SpecialType::BOOL);

constexpr r::Type BYTE_TYPE = r::Type(r::SpecialType::BYTE);

constexpr r::Type VOID_TYPE = r::Type(r::SpecialType::VOID);

constexpr r::Type VARIADIC_ARGUMENTS_TYPE = r::Type(r::SpecialType::VARIADIC_ARGUMENTS);

constexpr r::Type SIGNED_32_BIT_INTEGER_TYPE = r::Type(r::Integer(r::IntegerType::SIGNED, 32UZ));


bool operator==(const r::Subtype& lhs, const r::Subtype& rhs) noexcept;
bool operator!=(const r::Subtype& lhs, const r::Subtype& rhs) noexcept;

bool operator==(const r::Type& lhs, const r::Type& rhs) noexcept;
bool operator!=(const r::Type& lhs, const r::Type& rhs) noexcept;

bool get_types_are_equivalent(const r::Type& type_a, const r::Type& type_b);

}