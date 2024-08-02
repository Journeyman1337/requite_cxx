// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <type.hpp>
#include <expression.hpp>
#include <module/module.hpp>
#include <binary.hpp>
#include <symbol.hpp>
#include <symbol_table.hpp>
#include <binary.hpp>
#include <export_group.hpp>
#include <object.hpp>
#include <literal.hpp>

#include <variant>
#include <cassert>
#include <utility>
#include <stdexcept>
#include <ranges>
#include <array>
#include <cstddef>
#include <span>

namespace r {

Type::Type(r::TypeRoot root)
    : root(root)
{}

Type::Type(r::TypeRoot root, std::initializer_list<r::Subtype> subtypes)
    : root(root)
    , subtypes(subtypes)
{}

constexpr std::array<std::size_t, 21UZ> LITERAL_INT_MAX_VALUE_DIGIT_COUNT_MINUS_ONE =
    {
        2UZ,
        4UZ,
        9UZ,
        18UZ,
        38UZ,
        76UZ,
        153UZ,
        307UZ,
        616UZ,
        1232UZ,
        2465UZ,
        4931UZ,
        9863UZ,
        19396UZ,
        39456UZ,
        78912UZ,
        157826UZ,
        315652UZ,
        631305UZ,
        1262612UZ,
        2525222UZ
    };

constexpr std::array<std::size_t, 21UZ> INT_BIT_DEPTHS =
    {
        8UZ,
        16UZ,
        32UZ,
        64UZ,
        128UZ,
        256UZ,
        512UZ,
        1024UZ,
        2048UZ,
        4096UZ,
        8192UZ,
        16384UZ,
        32768UZ,
        65536UZ,
        131072UZ,
        262144UZ,
        524288UZ,
        1048576UZ,
        2097152UZ,
        4194304UZ,
        8388608UZ
    };

Type::Type(const r::Literal& literal)
{
    this->qualifiers.set(r::QualifierFlag::LITERAL);
    if (literal.type == r::LiteralType::CODEUNIT)
    {
        this->root = r::Codeunit(r::Encoding::ASCII);
    }
    else if (literal.type == r::LiteralType::STRING)
    {
        this->root = r::Codeunit(r::Encoding::ASCII);
        r::Subtype& subtype = this->subtypes.emplace_back();
        subtype.qualifiers.set(r::QualifierFlag::POINTER);
    }
    else if (literal.type == r::LiteralType::NUMBER)
    {
        const std::size_t text_length = literal.text.size();
        // this starts at 2UZ because want to use 32 bits at minimum.
        for (std::size_t size_i = 2UZ; size_i < 21UZ; size_i++)
        {
            const std::size_t max = r::LITERAL_INT_MAX_VALUE_DIGIT_COUNT_MINUS_ONE[size_i];
            if (text_length < max)
            {
                const std::size_t bit_depth = INT_BIT_DEPTHS[size_i];
                this->root =
                    r::Integer(
                        r::IntegerType::SIGNED,
                        bit_depth
                    );
                break;
            }
        }
        if (std::holds_alternative<std::monostate>(this->root))
        {
            throw std::runtime_error("literal too large.");
        }        
    }
    else if (literal.type == r::LiteralType::NUMBER_WITH_DECIMAL)
    {
        this->root = r::FloatingPoint(r::FloatingPointType::BINARY_SINGLE);
    }
}

void Type::add_pointer()
{
    r::Subtype& subtype = this->subtypes.emplace_back();
    subtype.qualifiers.set(r::QualifierFlag::POINTER);
}

bool Type::get_is_empty() const noexcept
{
    return std::holds_alternative<std::monostate>(this->root);
}

void Type::clear() noexcept
{
    this->root = {};
    this->qualifiers = {};
    this->subtypes.clear();
}

bool Type::get_is_primitive() const noexcept
{
    if (std::holds_alternative<r::Codeunit>(this->root))
    {
        return true;
    }
    else if (std::holds_alternative<r::Integer>(this->root))
    {
        return true;
    }
    else if (std::holds_alternative<r::FloatingPoint>(this->root))
    {
        return true;
    }
    else if (std::holds_alternative<r::FixedPoint>(this->root))
    {
        return true;
    }
    else if (!std::holds_alternative<r::SpecialType>(this->root))
    {
        return false;
    }
    r::SpecialType special_type = std::get<r::SpecialType>(this->root);
    if (special_type == r::SpecialType::BOOL)
    {
        return true;
    }
    else if (special_type == r::SpecialType::BYTE)
    {
        return true;
    }
    return false;
}

bool Type::get_is_special_type(r::SpecialType special_type) const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    else if (this->get_is_array())
    {
        return false;
    }
    else if (!std::holds_alternative<r::SpecialType>(this->root))
    {
        return false;
    }
    r::SpecialType root_special_type = std::get<r::SpecialType>(this->root);
    if (root_special_type == special_type)
    {
        return true;
    }
    return false;
}

bool Type::get_is_pointer_to_codeunit() const noexcept
{
    if (this->subtypes.size() != 1UZ)
    {
        return false;
    }
    if (!this->get_is_pointer())
    {
        return false;
    }
    else if (this->subtypes.size() != 1UZ)
    {
        return false;
    }
    else if (!std::holds_alternative<r::Codeunit>(this->root))
    {
        return false;
    }
    return true;
}

bool Type::get_is_byte() const noexcept
{
    if (!std::holds_alternative<r::SpecialType>(this->root))
    {
        return false;
    }
    r::SpecialType special_type = std::get<r::SpecialType>(this->root);
    if (special_type == r::SpecialType::BYTE)
    {
        return true;
    }
    return false;
}

bool Type::get_is_numeric_primitive() const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    else if (std::holds_alternative<r::SpecialType>(this->root))
    {
        return false;
    }
    else if (std::holds_alternative<r::Integer>(this->root))
    {
        return true;
    }
    else if (std::holds_alternative<r::FixedPoint>(this->root))
    {
        return true;
    }
    else if (std::holds_alternative<r::FloatingPoint>(this->root))
    {
        return true;
    }
    return false;
}


bool Type::get_is_integer() const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    if (this->get_is_array())
    {
        return false;
    }
    if (std::holds_alternative<r::Integer>(this->root))
    {
        return true;
    }  
    return false;
}

bool Type::get_is_signed_integer() const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    if (this->get_is_array())
    {
        return false;
    }
    if (!std::holds_alternative<r::Integer>(this->root))
    {
        return false;
    }  
    const r::Integer integer = std::get<r::Integer>(this->root);
    if (integer.type == r::IntegerType::SIGNED)
    {
        return true;
    }
    return false;
}

bool Type::get_is_unsigned_integer() const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    if (this->get_is_array())
    {
        return false;
    }
    if (!std::holds_alternative<r::Integer>(this->root))
    {
        return false;
    }  
    const r::Integer integer = std::get<r::Integer>(this->root);
    if (integer.type == r::IntegerType::UNSIGNED)
    {
        return true;
    }
    return false;
}

bool Type::get_is_floating_point() const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    if (this->get_is_array())
    {
        return false;
    }
    if (std::holds_alternative<r::FloatingPoint>(this->root))
    {
        return true;
    }  
    return false;
}

bool Type::get_is_void() const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    if (!std::holds_alternative<r::SpecialType>(this->root))
    {
        return false;
    }  
    r::SpecialType special_type = std::get<r::SpecialType>(this->root);
    if (special_type == r::SpecialType::VOID)
    {
        return true;
    }
    return false;
}

bool Type::get_is_bool() const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    if (!std::holds_alternative<r::SpecialType>(this->root))
    {
        return false;
    }  
    r::SpecialType special_type = std::get<r::SpecialType>(this->root);
    if (special_type == r::SpecialType::BOOL)
    {
        return true;
    }
    return false;
}

bool Type::get_is_pointer() const noexcept
{
    if (this->subtypes.empty())
    {
        if (std::holds_alternative<r::SpecialType>(this->root))
        {
            r::SpecialType special_type = std::get<r::SpecialType>(this->root);
            if (special_type == r::SpecialType::_NULL)
            {
                return true;    
            }
        }
        return false;
    }
    const r::Subtype& front_subtype = this->subtypes.front();
    if (!front_subtype.qualifiers.test(r::QualifierFlag::POINTER))
    {
        return false;
    }
    return true;
}

bool Type::get_is_null() const noexcept
{
    if (!this->subtypes.empty())
    {
        return false;
    }
    return this->get_has_null_root();
}

bool Type::get_has_null_root() const noexcept
{
    if (!std::holds_alternative<r::SpecialType>(this->root))
    {
        return false;
    }  
    r::SpecialType special_type = std::get<r::SpecialType>(this->root);
    if (special_type == r::SpecialType::_NULL)
    {
        return true;
    }
    return false;
}

bool Type::get_is_array() const noexcept
{
    if (this->subtypes.empty())
    {
        return false;
    }
    const r::Subtype& front_subtype = this->subtypes.front();
    if (!front_subtype.qualifiers.test(r::QualifierFlag::ARRAY))
    {
        return false;
    }
    return true;
}

std::size_t Type::get_array_size() const noexcept
{
    if (this->subtypes.empty())
    {
        return 0UZ;
    }
    const r::Subtype& front_subtype = this->subtypes.front();
    return front_subtype.array_size;
}

bool Type::get_is_indexable() const noexcept
{
    if (this->get_is_pointer())
    {
        return true;
    }
    else if (this->get_is_array())
    {
        return true;
    }
    return false;
}

bool Type::get_is_endemic() const noexcept
{
    return this->subtypes.empty();
}

bool Type::get_is_object() const noexcept
{
    if (!this->get_is_endemic())
    {
        return false;
    }
    if (std::holds_alternative<r::Object*>(this->root))
    {
        return true;
    }
    return false;
}

bool Type::get_is_mutable() const noexcept
{
    if (this->subtypes.empty())
    {
        return this->qualifiers.test(r::QualifierFlag::MUTABLE);
    }
    else
    {
        return this->subtypes.back().qualifiers.test(r::QualifierFlag::MUTABLE);
    }
}

bool Type::get_is_volatile() const noexcept
{
    if (this->subtypes.empty())
    {
        return this->qualifiers.test(r::QualifierFlag::VOLATILE);
    }
    else
    {
        return this->subtypes.back().qualifiers.test(r::QualifierFlag::VOLATILE);
    }
}

r::QualifierFlagSet& Type::get_top_qualifiers() noexcept
{
    if (this->subtypes.empty())
    {
        return this->qualifiers;
    }
    else
    {
        return this->subtypes.back().qualifiers;
    }
}

const r::QualifierFlagSet& Type::get_top_qualifiers() const noexcept
{
    if (this->subtypes.empty())
    {
        return this->qualifiers;
    }
    else
    {
        return this->subtypes.back().qualifiers;
    }
}

void Type::set_mutable(bool mutability) noexcept
{
    r::QualifierFlagSet& qualifiers = this->get_top_qualifiers();
    if (mutability)
    {
        qualifiers.set(r::QualifierFlag::MUTABLE);
    }
    else
    {
        qualifiers.reset(r::QualifierFlag::MUTABLE);
    }
}

void Type::set_volatile(bool volatility) noexcept
{
    r::QualifierFlagSet& qualifiers = this->get_top_qualifiers();
    if (volatility)
    {
        qualifiers.set(r::QualifierFlag::VOLATILE);
    }
    else
    {
        qualifiers.reset(r::QualifierFlag::VOLATILE);
    }
}

bool Type::get_is_literal() const noexcept
{
    return
        this->subtypes.empty() &&
        this->qualifiers.test(r::QualifierFlag::LITERAL);
}

void Type::clear_literals() noexcept
{
    this->qualifiers.reset(r::QualifierFlag::LITERAL);
}

bool Type::get_is_type_alias() const noexcept
{
    return std::holds_alternative<r::TypeAlias*>(this->root);
}

void Type::resolve_type_alias() noexcept
{
    while (std::holds_alternative<r::TypeAlias*>(this->root))
    {
        r::TypeAlias* alias = std::get<r::TypeAlias*>(this->root);
        std::size_t old_subtypes_size = this->subtypes.size();
        r::SubtypeVector& alias_subtypes = alias->type.subtypes;
        this->subtypes.insert(
            this->subtypes.end(),
            alias_subtypes.begin(),
            alias_subtypes.end()
        );
        this->root = alias->type.root;
        if (!alias_subtypes.empty())
        {
            r::Subtype& qualified_subtype = this->subtypes[old_subtypes_size];
            qualified_subtype.qualifiers |= this->qualifiers;
            this->qualifiers = alias->type.qualifiers;
        }
        else
        {
            this->qualifiers |= alias->type.qualifiers;
        }

    }
}

r::Object& Type::get_object() noexcept
{
    assert(this->get_is_object());
    r::Object* object_ptr = std::get<r::Object*>(this->root);
    assert(object_ptr != nullptr);
    return *object_ptr;
}

bool operator==(const r::Subtype& lhs, const r::Subtype& rhs) noexcept
{
    return 
        lhs.qualifiers == rhs.qualifiers &&
        lhs.array_size == rhs.array_size;
}

bool operator!=(const r::Subtype& lhs, const r::Subtype& rhs) noexcept
{
    return 
        lhs.qualifiers != rhs.qualifiers ||
        lhs.array_size != rhs.array_size;
}

bool operator==(const r::Type& lhs, const r::Type& rhs) noexcept
{
    return
        lhs.root == rhs.root &&
        lhs.qualifiers == rhs.qualifiers &&
        lhs.subtypes == rhs.subtypes;
}

bool operator!=(const r::Type& lhs, const r::Type& rhs) noexcept
{
    return
        lhs.root != rhs.root ||
        lhs.qualifiers != rhs.qualifiers ||
        lhs.subtypes != rhs.subtypes;
}

bool get_types_are_equivalent(const r::Type& type_a, const r::Type& type_b)
{
    assert(!type_a.get_is_literal());
    assert(!type_b.get_is_literal());
    if (type_a.subtypes.size() != type_b.subtypes.size())
    {
        return false;
    }
    for (std::size_t subtype_i = 0UZ; subtype_i < type_a.subtypes.size(); subtype_i++)
    {
        const r::Subtype& subtype_a = type_a.subtypes[subtype_i];
        const r::Subtype& subtype_b = type_b.subtypes[subtype_i];
        if (subtype_a.qualifiers.test(r::QualifierFlag::MUTABLE) != subtype_b.qualifiers.test(r::QualifierFlag::MUTABLE))
        {
            return false;
        }
    }
    if (type_a.root != type_b.root)
    {
        return false;
    }
    return true;
}

bool Type::get_is_llvm_value_type() const noexcept
{
    if (this->get_is_primitive())
    {
        return true;
    }
    return false;
}

bool Type::get_is_llvm_store_type() const noexcept
{
    return !this->get_is_llvm_value_type();
}

}