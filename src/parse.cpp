// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <requite/ast/ast.hpp>

#include <cassert>
#include <cstddef>
#include <cctype>

namespace requite {

bool Ast::parse(std::string_view source)
{
    assert(this->expressions.empty());
    if (source.empty())
    {
        return true;
    }
    struct Parser final
    {
        std::size_t char_i;
        char cur_char;
        std::string_view source;
        std::vector<std::vector<requite::Expression>*> expressions_ptr_stack;

        Parser(std::vector<requite::Expression>* expressions_ptr, std::string_view source)
            : char_i(0)
            , cur_char(source[0])
            , source(source)
            , expressions_ptr(expressions_ptr)
        {
            assert(!source.empty());
        }

        bool get_is_at_end()
        {
            return this->char_i >= this->source.size();
        }

        void iterate_char()
        {
            assert(!this->get_is_at_end());
            this->cur_char = this->source[++this->char_i];
        }

        void skip_whitespace()
        {
            this->iterate_char();
            while (!this->get_is_at_end())
            {
                if (!std::isspace(this->cur_char))
                {
                    break;
                }
                this->iterate_char();
            }
        }

        void skip_line()
        {
            bool newline_found = false;
            while(!this->get_is_at_end())
            {
                if (this->cur_char == '\n' || this->cur_char == '\r')
                {
                    newline_found = true;
                    continue;
                }
                if (newline_found)
                {
                    break;
                }
            }
        }

        requite::Expression& get_new_expression()
        {
            assert(!this->expressions_ptr_stack.empty());
            assert(this->expressions_ptr_stack.back() != nullptr);
            return this->expressions_ptr_stack.back()->emplace_back();
        }

        void parse()
        {
            while (!this->get_is_at_end())
            {
                switch (this->cur_char)
                {
                    case ';':
                        this->skip_line();
                        break;
                    case '[':
                    {
                        requite::Expression& expression = this->get_new_expression();
                        expression.type = requite::ExpressionType::OPERATION;
                        this->skip_whitespace();
                        std::string_view opcode_word = this->parse_word();
                        expression.operation.opcode = requite::get_opcode(opcode_word);
                        this->expressions_ptr_stack.push_back(expression.operation.expressions_uptr.get());
                    }
                        break;
                    case ']':
                        this->leave_operation();
                        break;
                    default:
                        if (std::isdigit(this->cur_char))
                        {
                            requite::Expression& expression = this->get_new_expression();
                            expression.type = requite::ExpressionType::LITERAL;
                            expression.literal.text = this->parse_literal();
                        }
                        else if (std::isalpha(this->cur_char))
                        {
                            requite::Expression& expression = this->get_new_expression();
                            expression.type = requite::ExpressionType::IDENTIFIER;
                            expression.identifier.text = this->parse_word();
                        }
                }
                this->iterate_char();
            }
        }
    };
}

}