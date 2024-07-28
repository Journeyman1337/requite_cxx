// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <resolver/resolver.hpp>
#include <binary.hpp>
#include <type.hpp>
#include <literal.hpp>
#include <procedure_group.hpp>
#include <module/module.hpp>
#include <string_utility.hpp>
#include <utility.hpp>
#include <local.hpp>
#include <builder/builder.hpp>
#include <procedure.hpp>
#include <type_alias.hpp>

#include <optional>
#include <stdexcept>
#include <utility>
#include <stdexcept>
#include <ranges>
#include <cstddef>

namespace r {

bool Resolver::get_is_type_assignable_to_type(const r::Type& from, const r::Type& to)
{
    assert(!to.get_is_literal());
    if (from.subtypes.size() != to.subtypes.size())
    {
        return false;
    }
    for (std::size_t subtype_i = 0UZ; subtype_i < from.subtypes.size(); subtype_i++)
    {
        const r::Subtype& from_subtype = from.subtypes.at(subtype_i);
        const r::Subtype& to_subtype = to.subtypes.at(subtype_i);
        if (from_subtype.qualifiers.test(r::QualifierFlag::POINTER) != to_subtype.qualifiers.test(r::QualifierFlag::POINTER))
        {
            return false;
        }
        if (from_subtype.qualifiers.test(r::QualifierFlag::ARRAY) != to_subtype.qualifiers.test(r::QualifierFlag::ARRAY))
        {
            return false;
        }
    }
    if (from.get_is_literal())
    {
        if (from.get_is_integer())
        {
            if (!to.get_is_integer() && !to.get_is_floating_point())
            {
                return false;
            }
            if (to.get_is_integer())
            {
                if (this->get_bit_depth(from) > this->get_bit_depth(to))
                {
                    return false;
                }
            }
        }
        else if (from.get_is_floating_point())
        {
            if (!to.get_is_floating_point())
            {
                return false;
            }
        }
    }
    else if (to.root != to.root)
    {
        return false;
    }
    return true;
}

void Resolver::check_type_assignable_to_type(const r::Type& from, const r::Type& to)
{
    if (!this->get_is_type_assignable_to_type(from, to))
    {
        throw std::runtime_error("invalid type.");
    }
}

void Resolver::check_dereferenced_type_assignable_to_type(const r::Type& from, const r::Type& to)
{
    r::Type from_copy = from;
    if (!from_copy.get_is_pointer())
    {
        throw std::runtime_error("type not pointer.");
    }
    from_copy.subtypes.pop_back();
    this->check_type_assignable_to_type(from_copy, to);
}

void Resolver::check_indexed_type_assignable_to_type(const r::Type& from, const r::Type& to)
{
    r::Type from_copy = from;
    if (!from_copy.get_is_array())
    {
        throw std::runtime_error("type not array.");
    }
    from_copy.subtypes.pop_back();
    this->check_type_assignable_to_type(from_copy, to);
}

std::size_t Resolver::get_bit_depth(const r::Type& type)
{
    if (type.get_is_pointer())
    {
        return this->get_pointer_bit_depth();
    }
    else if (type.get_is_array())
    {
        r::Type copy = type;
        copy.subtypes.pop_back();
        return this->get_bit_depth(type) * type.get_array_size();
    }
    else if (std::holds_alternative<r::Codeunit>(type.root))
    {
        const r::Codeunit& codeunit = std::get<r::Codeunit>(type.root);
        return codeunit.get_bit_depth();
    }
    else if (std::holds_alternative<r::Integer>(type.root))
    {
        const r::Integer& integer = std::get<r::Integer>(type.root);
        return integer.bit_depth;
    }
    else if (std::holds_alternative<r::FloatingPoint>(type.root))
    {
        const r::FloatingPoint& floating_point = std::get<r::FloatingPoint>(type.root);
        return floating_point.get_bit_depth();
    }
    else if (std::holds_alternative<r::FixedPoint>(type.root))
    {
        const r::FixedPoint& fixed_point = std::get<r::FixedPoint>(type.root);
        return fixed_point.get_bit_depth();
    }
    else if (std::holds_alternative<r::SpecialType>(type.root))
    {
        r::SpecialType special_type = std::get<r::SpecialType>(type.root);
        return r::get_bit_depth(special_type);
    }
    else if (std::holds_alternative<r::Object*>(type.root))
    {
        r::Object& object = *std::get<r::Object*>(type.root);
        // TODO
    }
    r::unreachable();
}

std::size_t Resolver::get_byte_size(const r::Type& type)
{
    return this->get_bit_depth(type) * 8UZ;
}

r::Type Resolver::resolve_type(const r::Expression& expression, bool can_fail)
{
    r::Type type;
    const r::Expression* expression_ptr = &expression;
    const auto catalog_subtypes =
        [&]()
        {
            bool qualifier_is_mutable = false;
            bool qualifier_is_volatile = false;
            while (std::holds_alternative<r::Operation>(*expression_ptr))
            {
                const r::Operation& operation = std::get<r::Operation>(*expression_ptr);
                if (operation.opcode == r::Opcode::STAR)
                { // pointers
                    r::Subtype& next_subtype = type.subtypes.emplace_back();
                    next_subtype.qualifiers.set(r::QualifierFlag::POINTER, true);
                    next_subtype.qualifiers.set(r::QualifierFlag::MUTABLE, qualifier_is_mutable);
                    next_subtype.qualifiers.set(r::QualifierFlag::VOLATILE, qualifier_is_volatile);
                    qualifier_is_mutable = false;
                    qualifier_is_volatile = false;
                    assert(!operation.branches.empty());
                    expression_ptr = &operation.branches.back();
                }
                else if (operation.opcode == r::Opcode::BUILTIN_ARRAY)
                { // arrays
                    r::Subtype& next_subtype = type.subtypes.emplace_back();
                    next_subtype.qualifiers.set(r::QualifierFlag::ARRAY, true);
                    assert(operation.branches.size() == 2UZ);
                    const r::Expression& size_expression = operation.branches.back();
                    assert(std::holds_alternative<r::Literal>(size_expression));
                    const r::Literal& size_literal = std::get<r::Literal>(size_expression);
                    assert(size_literal.type == r::LiteralType::NUMBER);
                    next_subtype.array_size = r::to_number(size_literal.text);
                    next_subtype.qualifiers.set(r::QualifierFlag::MUTABLE, qualifier_is_mutable);
                    next_subtype.qualifiers.set(r::QualifierFlag::VOLATILE, qualifier_is_volatile);
                    qualifier_is_mutable = false;
                    qualifier_is_volatile = false;
                    expression_ptr = &operation.branches.front();
                }
                else if (operation.opcode == r::Opcode::BANG)
                { // mutable
                    assert(!qualifier_is_mutable);
                    qualifier_is_mutable = true;
                    assert(!operation.branches.empty());
                    expression_ptr = &operation.branches.back();
                }
                else if (operation.opcode == r::Opcode::QUESTION)
                { // volatile
                    assert(!qualifier_is_volatile);
                    qualifier_is_volatile = true;
                    assert(!operation.branches.empty());
                    expression_ptr = &operation.branches.back();
                }
                else
                {
                    break;
                }          
            }
        };
    const auto catalog_root_symbol =
        [&]()
        {
            r::Symbol* symbol_ptr = this->try_get_symbol(*expression_ptr);
            if (symbol_ptr == nullptr)
            {
                if (!can_fail)
                {
                    throw std::runtime_error("type not found.");
                }
                return;
            }
            r::Symbol& symbol = *symbol_ptr;
            if (std::holds_alternative<r::ProcedureGroup*>(symbol))
            {
                if (!can_fail)
                {
                    throw std::runtime_error("procedure is not a type.");
                }
            }
            else if (std::holds_alternative<r::Global*>(symbol))
            {
                if (!can_fail)
                {
                    throw std::runtime_error("global is not a type.");
                }
            }
            else if (std::holds_alternative<r::Object*>(symbol))
            {
                r::Object* object_ptr = std::get<r::Object*>(symbol);
                type.root = object_ptr;
            }
            else if (std::holds_alternative<r::ExportGroup*>(symbol))
            {
                if (!can_fail)
                {
                    throw std::runtime_error("export group is not a type.");
                }
            }
            else if (std::holds_alternative<r::TypeAlias*>(symbol))
            {
                r::TypeAlias* type_alias_ptr = std::get<r::TypeAlias*>(symbol);
                type.root = type_alias_ptr;
            }
            else
            {
                r::unreachable();
            }
        };
    const auto catalog_root =
        [&]()
        {
            if (std::holds_alternative<r::Operation>(*expression_ptr))
            {
                const r::Operation& operation = std::get<r::Operation>(*expression_ptr);
                if (operation.opcode == r::Opcode::BUILTIN_BOOL)
                {
                    type.root = r::SpecialType::BOOL;
                }
                else if (operation.opcode == r::Opcode::BUILTIN_BYTE)
                {
                    type.root = r::SpecialType::BYTE;
                }
                else if (operation.opcode == r::Opcode::BUILTIN_VARIADIC_ARGUMENTS)
                {
                    type.root = r::SpecialType::VARIADIC_ARGUMENTS;
                }
                else if (operation.opcode == r::Opcode::BUILTIN_VOID)
                {
                    type.root = r::SpecialType::VOID;
                }
                else if (operation.opcode == r::Opcode::BUILTIN_INTEGER)
                {
                    assert(operation.branches.size() == 2UZ);
                    const r::Expression& integer_type_expression = operation.branches.front();
                    assert(std::holds_alternative<std::string_view>(integer_type_expression));
                    std::string_view integer_type_text = std::get<std::string_view>(integer_type_expression);
                    r::IntegerType integer_type = r::to_integer_type(integer_type_text);
                    assert(integer_type != r::IntegerType::UNKNOWN);
                    const r::Expression& bit_depth_expression = operation.branches.back();
                    assert(std::holds_alternative<r::Literal>(bit_depth_expression));
                    r::Literal bit_depth_literal = std::get<r::Literal>(bit_depth_expression);
                    std::uint64_t bit_depth = r::to_number(bit_depth_literal.text);
                    if (!r::get_is_valid_integer_bit_depth(bit_depth))
                    {
                        throw std::runtime_error("invalid integer bit depth.");
                    }
                    type.root =
                        r::Integer(
                            integer_type,
                            bit_depth
                        );
                }
                else if (operation.opcode == r::Opcode::BUILTIN_FLOATING_POINT)
                {
                    assert(operation.branches.size() == 1UZ);
                    const r::Expression& floating_point_type_expression = operation.branches.front();
                    assert(std::holds_alternative<std::string_view>(floating_point_type_expression));
                    std::string_view floating_point_type_text = std::get<std::string_view>(floating_point_type_expression);
                    r::FloatingPointType floating_point_type = r::to_floating_point_type(floating_point_type_text);
                    assert(floating_point_type != r::FloatingPointType::UNKNOWN);
                    type.root =
                        r::FloatingPoint(
                            floating_point_type
                        );
                }
                else if (operation.opcode == r::Opcode::BUILTIN_FIXED_POINT)
                {
                    assert(operation.branches.size() == 2UZ);
                    const r::Expression& integer_bits_expression = operation.branches.front();
                    assert(std::holds_alternative<r::Literal>(integer_bits_expression));
                    const r::Literal& integer_bits_literal = std::get<r::Literal>(integer_bits_expression);
                    assert(integer_bits_literal.type == r::LiteralType::NUMBER);
                    std::uint64_t integer_bits = r::to_number(integer_bits_literal.text);
                    const r::Expression& decimal_bits_expression = operation.branches.back();
                    assert(std::holds_alternative<r::Literal>(decimal_bits_expression));
                    const r::Literal& decimal_bits_literal = std::get<r::Literal>(decimal_bits_expression);
                    assert(decimal_bits_literal.type == r::LiteralType::NUMBER);
                    std::uint64_t decimal_bits = r::to_number(decimal_bits_literal.text);
                    type.root =
                        r::FixedPoint(
                            integer_bits,
                            decimal_bits
                        );
                }
                else if (operation.opcode == r::Opcode::BUILTIN_CODEUNIT)
                {
                    assert(operation.branches.size() == 1UZ);
                    const r::Expression& encoding_expression = operation.branches.front();
                    assert(std::holds_alternative<std::string_view>(encoding_expression));
                    std::string_view encoding_text = std::get<std::string_view>(encoding_expression);
                    r::Encoding encoding = r::to_encoding(encoding_text);
                    assert(encoding != r::Encoding::UNKNOWN);
                    type.root =
                        r::Codeunit(
                            encoding
                        );
                }
                else if (operation.opcode == r::Opcode::ACCESS_TABLE)
                {
                    expression_ptr = &this->resolve_table(operation);
                    catalog_root_symbol();
                }
                //else if (operation.opcode == r::Opcode::TYPE_OF)
                //{
                //    
                //}
                //else if (operation.opcode == r::Opcode::ROOT_TYPE_OF)
                //{
                //
                //}
                //else if (operation.opcode == r::Opcode::SELF)
                //{
                //
                //}
                else
                {
                    catalog_root_symbol();
                }
            }
            else
            {
                catalog_root_symbol();
            }
        };

    if (std::holds_alternative<r::Operation>(expression))
    {
        const r::Operation& operation = std::get<r::Operation>(expression);
        if (operation.opcode == r::Opcode::ACCESS_TABLE)
        {
            assert(operation.branches.size() >= 2UZ);
            expression_ptr = &this->resolve_table(operation);
        }
    }
    catalog_subtypes();
    catalog_root();    

    this->reset_table();
    return type;
}

r::Type Resolver::deduce_type(const r::Expression& expression, r::Builder* builder)
{
    if (builder != nullptr)
    {
        if (std::holds_alternative<std::string_view>(expression))
        {
            std::string_view name = std::get<std::string_view>(expression);
            r::Local* local = builder->try_get_local(name);
            if (local == nullptr)
            {
                throw std::runtime_error("variable not found with name.");
            }
            return local->type;
        }
    }
    if (std::holds_alternative<r::Operation>(expression))
    {
        const r::Operation& operation = std::get<r::Operation>(expression);
        if (operation.opcode == r::Opcode::ACCESS_MEMBER)
        {
            assert(operation.branches.size() > 1UZ);
            r::Object* object = nullptr;
            for (const r::Expression& branch_expression : operation.branches | std::views::take(operation.branches.size() - 1UZ))
            {
                r::Type branch_type = this->deduce_type(branch_expression, builder);
                assert(std::holds_alternative<r::Object*>(branch_type.root));
                object = std::get<r::Object*>(branch_type.root);
            }
            const r::Expression& last_expression = operation.branches.back();
            assert(std::holds_alternative<std::string_view>(last_expression));
            std::string_view property_name = std::get<std::string_view>(last_expression);
            r::Property& property = object->get_property(property_name);
            return property.type;
        }
        if (operation.opcode == r::Opcode::CONSTRUCT)
        {
            assert(!operation.branches.empty());
            const r::Expression& type_expression = operation.branches.front();
            return this->resolve_type(type_expression);
        }
        if (operation.opcode == r::Opcode::LOCAL)
        {
            assert(operation.branches.size() >= 2UZ);
            const r::Expression& type_expression = operation.branches.at(1UZ);
            return this->resolve_type(type_expression);
        }
        if (operation.opcode == r::Opcode::CALL)
        {
            assert(!operation.branches.empty());
            r::Procedure& procedure = this->get_call_procedure(operation);
            return procedure.return_type;
        }
        if (operation.opcode == r::Opcode::DEREFERENCE)
        {
            assert(operation.branches.size() == 1UZ);
            const r::Expression& dereferenced_expression = operation.branches.front();
            r::Type type = this->deduce_type(dereferenced_expression, builder);
            assert(type.get_is_pointer());
            type.subtypes.pop_back();
            return type;
        }
        if (operation.opcode == r::Opcode::ACCESS_VARIADIC_ARGUMENT)
        {
            assert(operation.branches.size() == 2UZ);
            const r::Expression& type_expression = operation.branches.front();
            r::Type type = this->resolve_type(type_expression);
            return type;
        }
        if (operation.opcode == r::Opcode::INDEX_INTO)
        {
            assert(operation.branches.size() == 2UZ);
            const r::Expression& array_variable_expression = operation.branches.front();
            r::Type type = this->deduce_type(array_variable_expression, builder);
            assert(type.get_is_indexable());
            type.subtypes.pop_back();
            return type;
        }
        if (operation.opcode == r::Opcode::THIS)
        {
            assert(operation.branches.empty());
            r::Type object_type = r::Type(this->object);
            object_type.add_pointer();
            return object_type;
        }
        if (r::get_is_math_opcode(operation.opcode))
        {
            return this->deduce_group_type(operation.branches, builder);
        }
        if (r::get_opcode_returns_bool(operation.opcode))
        {
            return r::BOOL_TYPE;
        }
        if (operation.opcode == r::Opcode::TRUNCATE)
        {
            assert(operation.branches.size() == 1UZ);
            // TODO determine best type for the argument 
            // assert(type.get_is_float());
            // return type;
        }
        // TODO every other expression opcode
    }
    else if (std::holds_alternative<r::Literal>(expression))
    {
        const r::Literal& literal = std::get<r::Literal>(expression);
        return r::Type(literal);
    }
    r::unreachable();
}

r::Type Resolver::deduce_group_type(std::span<const r::Expression> branch_group, r::Builder* builder)
{ // deduce the common type of arguments passed to a group operation that expects multiple of the same type. (+, -, *, %, ==, >=, list, etc)
    if (branch_group.empty())
    {
        return r::Type();
    }
    r::Type type = this->deduce_type(branch_group.front(), builder);
    for (const r::Expression& expression : branch_group | std::views::drop(1UZ))
    {
        r::Type next_type = this->deduce_type(expression, builder);
        type = this->deduce_group_type(type, next_type);
    }
    return type;
}

r::Type Resolver::deduce_group_type(const r::Type& type_a, const r::Type& type_b)
{ // NOTE: dont care about mutability and volatility here. this is for lvalues only.
    if (type_a == type_b)
    {
        return type_a;
    }
    // NOTE: literals never have subtypes.
    if (type_a.get_is_literal() && type_b.get_is_literal())
    { // both are literals.
        if (type_a.get_is_floating_point() || type_b.get_is_floating_point())
        { // float literals are always f32.
            return type_a; 
        }
        if (type_a.get_is_integer() && type_b.get_is_integer())
        { // int literals are always signed.
            const r::Integer& integer_a = std::get<r::Integer>(type_a.root);
            const r::Integer& integer_b = std::get<r::Integer>(type_b.root);
            if (integer_a.bit_depth < integer_b.bit_depth)
            {
                return type_b;
            }
            else
            {
                return type_a;
            }
        }
    }
    if (type_a.get_is_literal() || type_b.get_is_literal())
    { // only one is a literal.
        const r::Type& literal_type = type_a.get_is_literal() ? type_a : type_b;
        const r::Type& non_literal_type = type_a.get_is_literal() ? type_b : type_a;
        if (non_literal_type.get_is_floating_point())
        {
            if (literal_type.get_is_integer() || literal_type.get_is_floating_point())
            {
                return non_literal_type;
            }
        }
        if (non_literal_type.get_is_integer() && literal_type.get_is_integer())
        {
            const r::Integer& literal_integer = std::get<r::Integer>(literal_type.root);
            const r::Integer& non_literal_integer = std::get<r::Integer>(non_literal_type.root);
            if (non_literal_integer.bit_depth >= literal_integer.bit_depth)
            {
                return non_literal_type;
            }
        }
    }
    // TODO object types.
    throw std::runtime_error("no common type found.");
}

}