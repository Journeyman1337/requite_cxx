// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <resolver/resolver.hpp>
#include <expression.hpp>
#include <type.hpp>

#include <llvm/ADT/APSInt.h>

namespace r {

llvm::APSInt Resolver::get_integer_constant(const r::Expression& expression, const r::Type& expected_type)
{
    assert(expected_type.get_is_integer());
    const r::Expression* cur_expression = &expression;
    bool is_negative = false;
    bool at_least_one_negative = false;
    while (std::holds_alternative<r::Operation>(*cur_expression))
    {
        const r::Operation& cur_operation = std::get<r::Operation>(*cur_expression);
        if (cur_operation.opcode == r::Opcode::MINUS)
        {
            at_least_one_negative = true;
            is_negative = !is_negative;
        }
        else if (cur_operation.opcode != r::Opcode::PLUS)
        {
            break;
        }
        assert(cur_operation.branches.size() == 1UZ);
        cur_expression = &cur_operation.branches.front();
    }
    if (std::holds_alternative<r::Operation>(*cur_expression))
    {
        const r::Operation& cur_operation = std::get<r::Operation>(*cur_expression);
        if (cur_operation.opcode == r::Opcode::POINTER_DEPTH)
        {
            assert(cur_operation.branches.empty());
            assert(!at_least_one_negative);
            std::size_t pointer_depth = this->get_pointer_bit_depth();
            r::Type uptr_type = this->get_uptr_type();
            this->check_type_assignable_to_type(uptr_type, expected_type);
            return
                llvm::APSInt(
                    llvm::APInt(
                        pointer_depth,
                        std::to_array({static_cast<std::uint64_t>(pointer_depth)})
                    ),
                    false
                );
        }
    }
    assert(std::holds_alternative<r::Literal>(*cur_expression));
    const r::Literal& literal = std::get<r::Literal>(*cur_expression);
    assert(literal.type == r::LiteralType::NUMBER);
    r::Integer integer = std::get<r::Integer>(expected_type.root);
    llvm::APInt llvm_ap_int =
        llvm::APInt(
            integer.bit_depth,
            literal.text,
            10
        );
    if (is_negative)
    {
        llvm_ap_int.flipAllBits();
    }
    return 
        llvm::APSInt(
            llvm_ap_int,
            is_negative
        );
}

}