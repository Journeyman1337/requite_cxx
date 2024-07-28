// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <module/module.hpp>

#include <cstddef>
#include <utility>
#include <cassert>
#include <optional>

namespace r {

void Module::parse_ast()
{
    struct Parser
    {
        std::string_view source;
        std::size_t char_i = 0UZ;

        Parser(std::string_view source)
            : source(source)
        {}

        char get_char(std::size_t char_i)
        {
            if (char_i >= this->source.size())
            {
                return '\0';
            }
            return this->source.at(char_i);
        }

        char get_cur_char()
        {
            return this->get_char(char_i);
        }           
           
        char get_next_char()
        {
            return this->get_char(this->char_i + 1UZ);
        }

        bool get_is_at_end()
        {
            return this->char_i >= this->source.size();
        }

        bool try_skip_newline()
        {
            assert(!this->get_is_at_end());
            if (this->get_cur_char() == '\r')
            {
                this->char_i++;
                if (this->get_cur_char() == '\n')
                {
                    this->char_i++;
                }
                return true;
            }
            else if (this->get_cur_char() == '\n')
            {
                this->char_i++;
                if (this->get_cur_char() == '\r')
                {
                    this->char_i++;
                }
                return true;
            }
            return false;
        }

        void skip_single_line_comment()
        {
            assert(!this->get_is_at_end());
            assert(this->get_cur_char());
            assert(this->get_next_char());
            while (!this->try_skip_newline() && !this->get_is_at_end())
            {
                this->char_i++;
            }
        }

        void skip_multi_line_comment()
        {
            assert(!this->get_is_at_end());
            assert(this->get_cur_char() == '/');
            assert(this->get_next_char() == '*');
            while (this->get_cur_char() != '*' && this->get_next_char() != '/')
            {
                if (this->get_is_at_end())
                {
                    throw std::runtime_error("multi-line comment must terminate.");
                }
                this->char_i++;
            }
            this->char_i += 2UZ;
        }
           
        void skip_whitespace()
        {
            while (std::isspace(this->get_cur_char()) && !this->get_is_at_end())
            {
                this->char_i++;
            }
        }

        void skip_comments_and_spaces()
        {
            while (!this->get_is_at_end())
            {
                switch (this->get_cur_char())
                {
                    case '/':
                        switch (this->get_next_char())
                        {
                            case '/':
                                this->skip_single_line_comment();
                                break;
                            case '*':
                                this->skip_multi_line_comment();
                                break;
                        }
                        break;
                    default:
                        if (std::isspace(this->get_cur_char()))
                        {
                            this->skip_whitespace();
                            break;
                        }
                        else
                        {
                            return;
                        }
                }
            }
        }
            
        std::string_view parse_quoted_literal(char quote)
        {
            assert(this->get_cur_char() == quote);
            const std::size_t start_i = this->char_i;
            while (!this->get_is_at_end())
            {
                this->char_i++;
                const char cur_char = this->get_cur_char();
                if (cur_char == '\\')
                {
                    this->char_i++;
                }
                else if (cur_char == quote)
                {
                    this->char_i++;
                    return 
                        std::string_view(
                            this->source.data() + start_i,
                            this->char_i - start_i
                        );
                }
            }
            throw std::runtime_error("quoted literal must terminate.");
        }
        
        bool get_is_word_start()
        {
            return std::isalpha(this->get_cur_char()) || this->get_cur_char() == '_';
        }

        std::string_view parse_word()
        {
            assert(!this->get_is_at_end());
            assert(this->get_is_word_start());
            const std::size_t start_i = this->char_i;
            while (!this->get_is_at_end())
            {
                this->char_i++;
                const char cur_char = this->get_cur_char();
                if (
                    !std::isalpha(cur_char) &&
                    !std::isdigit(cur_char) &&
                    cur_char != '_'
                )
                {
                    break;
                }
            }
            return 
                std::string_view(
                    this->source.data() + start_i,
                    this->char_i - start_i
                );
        }
    
        r::Literal parse_number()
        {
            assert(!this->get_is_at_end());
            assert(std::isdigit(this->get_cur_char()));
            bool found_decimal = false;
            const std::size_t start_i = this->char_i;
            while (!this->get_is_at_end())
            {
                const char cur_char = this->get_cur_char();
                if (cur_char == '.')
                {
                    if (found_decimal)
                    {
                        break;
                    }
                    found_decimal = true;
                }
                else if (!std::isdigit(cur_char))
                {
                    break;
                }
                char_i++;
            }
            r::Literal literal;
            literal.text = 
                std::string_view(
                    this->source.data() + start_i,
                    this->char_i - start_i
                );
            literal.type = (found_decimal) ? r::LiteralType::NUMBER_WITH_DECIMAL : r::LiteralType::NUMBER;
            return literal;
        }

        r::Literal parse_string()
        {
            assert(this->get_cur_char() == '\"');
            r::Literal literal;
            literal.text = this->parse_quoted_literal('\"');
            literal.type = r::LiteralType::STRING;
            return literal;
        }

        r::Literal parse_codeunit()
        {
            assert(this->get_cur_char() == '\'');
            r::Literal literal;
            literal.text = this->parse_quoted_literal('\'');
            literal.type = r::LiteralType::STRING;
            return literal;
        }

        r::Opcode parse_opcode()
        {
            assert(!std::isspace(this->get_cur_char()));
            while (!this->get_is_at_end())
            {
                switch (this->get_cur_char())
                {
                    case '/':
                        assert(this->get_next_char() != '/');
                        assert(this->get_next_char() != '*');
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::DIVIDE_EQUAL;
                            default:
                                this->char_i++;
                                return r::Opcode::DIVIDE;   
                        }
                        break;
                    case '+':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::PLUS_EQUAL;
                            default:
                                this->char_i++;
                                return r::Opcode::PLUS;   
                        }
                        break;
                    case '-':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::MINUS_EQUAL;
                            default:
                                this->char_i++;
                                return r::Opcode::MINUS;   
                        }
                        break;
                    case '*':
                            switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::STAR_EQUAL;
                            default:
                                this->char_i++;
                                return r::Opcode::STAR;   
                        }
                        break;
                    case '%':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::MODULUS_EQUAL;
                            default:
                                this->char_i++;
                                return r::Opcode::MODULUS;   
                        }
                        break;
                    case '=':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::EQUAL_EQUAL;
                            default:
                                this->char_i++;
                                return r::Opcode::EQUAL;   
                        }
                        break;
                    case '!':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::BANG_EQUAL;
                            default:
                                this->char_i++;
                                return r::Opcode::BANG;   
                        }
                        break;
                    case '^':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::CAROT_EQUAL;
                            default:
                                this->char_i++;
                                return r::Opcode::CAROT;   
                        }
                        break;
                    case '&':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::AND_EQUAL;
                            case '&':
                                this->char_i += 2UZ;
                                return r::Opcode::AND_AND;
                            default:
                                this->char_i++;
                                return r::Opcode::AND;   
                        }
                        break;
                    case '|':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::PIPE_EQUAL;
                            case '|':
                                this->char_i += 2UZ;
                                return r::Opcode::PIPE_PIPE;
                            default:
                                this->char_i++;
                                return r::Opcode::PIPE;   
                        }
                        break;
                    case '.':
                        this->char_i++;
                        return r::Opcode::ACCESS_MEMBER;
                    case ':':
                        this->char_i++;
                        return r::Opcode::ACCESS_TABLE;
                    case '<':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::LESS_EQUAL;
                            case '<':
                                this->char_i += 2UZ;
                                return r::Opcode::LESS_LESS;
                            default:
                                this->char_i++;
                                return r::Opcode::LESS;   
                        }
                        break;
                    case '>':
                        switch (this->get_next_char())
                        {
                            case '=':
                                this->char_i += 2UZ;
                                return r::Opcode::GREATER_EQUAL;
                            case '>':
                                this->char_i += 2UZ;
                                return r::Opcode::GREATER_GREATER;
                            default:
                                this->char_i++;
                                return r::Opcode::GREATER;   
                        }
                        break;
                    case '?':
                        this->char_i++;
                        return r::Opcode::QUESTION;
                    default:
                        if (std::isdigit(this->get_cur_char()))
                        {
                            throw std::runtime_error("opcode must not be a literal.");
                        }
                        if (this->get_is_word_start())
                        {
                            return 
                                r::to_opcode(
                                    this->parse_word()
                                );
                        }
                }
            }
            throw std::runtime_error("unexpected character.");
        }

        r::Operation parse_expression_args(char terminator, r::Operation& operation)
        {
            this->skip_comments_and_spaces();
            while (!this->get_is_at_end())
            {
                const char cur_char = this->get_cur_char();
                if (this->get_cur_char() == terminator)
                {
                    this->char_i++;
                    return operation;
                }
                r::Expression expression_arg = this->parse_expression();
                if (
                    !operation.branches.empty() &&
                    std::holds_alternative<r::Operation>(expression_arg)
                )
                {
                    r::Operation& operation_arg = std::get<r::Operation>(expression_arg);
                    if (
                        operation_arg.opcode == r::Opcode::ELSE_IF ||
                        operation_arg.opcode == r::Opcode::ELSE
                    )
                    {
                        r::Expression& prev_expression_arg = operation.branches.back();
                        if (std::holds_alternative<r::Operation>(prev_expression_arg))
                        {
                            r::Operation& prev_operation_arg = std::get<r::Operation>(prev_expression_arg);
                            if (prev_operation_arg.opcode == r::Opcode::CONDITION)
                            {
                                prev_operation_arg.branches.push_back(std::move(expression_arg));
                                continue;
                            }
                        }
                    }
                }
                operation.branches.push_back(std::move(expression_arg));
                // parse_expression() should also skip comments and spaces after.               
            }
            throw std::runtime_error("expression must terminate in closing character.");
        }

        r::Operation parse_expression_args(char terminator, r::Opcode opcode)
        {
            r::Operation operation;
            operation.opcode = opcode;
            return this->parse_expression_args(terminator, operation);
        }

        r::Operation parse_s_expression()
        {
            assert(this->get_cur_char() == '[');
            this->char_i++;
            this->skip_comments_and_spaces();
            r::Opcode opcode = this->parse_opcode();
            if (opcode == r::Opcode::IF)
            {
                r::Operation condition;
                condition.opcode = r::Opcode::CONDITION;
                condition.branches.push_back(
                    std::move(
                        this->parse_expression_args(']', opcode)
                    )
                );
                return condition;
            }
            return this->parse_expression_args(']', opcode);
        }

        r::Operation parse_unary_expression(r::Opcode opcode)
        {
            r::Operation operation;
            operation.opcode = opcode;
            this->skip_comments_and_spaces();
            operation.branches.push_back(this->parse_expression());
            return operation;
        }

        r::Expression parse_inner_expression()
        {
            this->skip_comments_and_spaces();
            assert(!this->get_is_at_end());
            switch (this->get_cur_char())
            {
                case '[':
                    return this->parse_s_expression();
                case '+':
                    this->char_i++;
                    return this->parse_unary_expression(r::Opcode::PLUS);
                case '-':
                    this->char_i++;
                    return this->parse_unary_expression(r::Opcode::MINUS);
                case '!':
                    if (this->get_next_char() == '!')
                    {
                        this->char_i += 2UZ;
                        return this->parse_unary_expression(r::Opcode::BANG_BANG);
                    }
                    this->char_i++;
                    return this->parse_unary_expression(r::Opcode::BANG);
                case '*':
                    this->char_i++;
                    return this->parse_unary_expression(r::Opcode::STAR);
                case '#':
                    this->char_i++;
                    return this->parse_unary_expression(r::Opcode::HASH);
                case '\"':
                    return this->parse_string();
                case '\'':
                    return this->parse_codeunit();
                default:
                    if (this->get_is_word_start())
                    {
                        const std::string_view word = this->parse_word();
                        return word;
                    }        
                    else if (std::isdigit(this->get_cur_char()))
                    {
                        const r::Literal literal = this->parse_number();
                        return literal;
                    }
            }
            const auto cur_char = this->get_cur_char();
            throw std::runtime_error("unexpected character.");
        }

        r::Expression parse_expression()
        {
            r::Expression expression = this->parse_inner_expression();
            this->skip_comments_and_spaces();
            while(!this->get_is_at_end())
            {
                const auto parse_binary_expression =
                    [&](r::Opcode opcode)
                    {
                        this->skip_comments_and_spaces();
                        r::Expression next_expression = this->parse_inner_expression();
                        if (std::holds_alternative<r::Operation>(expression))
                        {
                            r::Operation& operation = std::get<r::Operation>(expression);
                            if (operation.opcode == opcode)
                            {
                                operation.branches.push_back(
                                    std::move(
                                        next_expression
                                    )
                                );
                                return;
                            }
                        }
                        r::Operation binary_operation;
                        binary_operation.opcode = opcode;
                        binary_operation.branches.push_back(
                            std::move(
                                expression
                            )
                        );
                        binary_operation.branches.push_back(
                            std::move(
                                next_expression
                            )
                        );
                        expression = std::move(binary_operation);                
                    };
                const auto parse_m_expression =
                    [&](char terminator, r::Opcode opcode)
                    {
                        r::Operation operation;
                        operation.opcode = opcode;
                        operation.branches.emplace_back(std::move(expression));
                        this->parse_expression_args(terminator, operation);
                        expression = std::move(operation);
                    };
                switch (this->get_cur_char())
                {
                    case '.':
                        this->char_i++;
                        parse_binary_expression(r::Opcode::ACCESS_MEMBER);
                        break;
                    case ':':
                        this->char_i++;
                        parse_binary_expression(r::Opcode::ACCESS_TABLE);
                        break;
                    case '(':
                        this->char_i++;
                        parse_m_expression(')', r::Opcode::CALL);
                        break;
                    case '{':
                        this->char_i++;
                        parse_m_expression('}', r::Opcode::CONSTRUCT);
                        break;
                    case ',':
                        this->char_i++;
                        parse_binary_expression(r::Opcode::ATTRIBUTES);
                        break;
                    default:
                        return expression;
                }   
                this->skip_comments_and_spaces();         
            }
            return expression;
        }
    };
    
    Parser parser(source);
    parser.skip_comments_and_spaces();
    while (!parser.get_is_at_end())
    {
        r::Expression expression = parser.parse_expression();
        if (std::holds_alternative<std::string_view>(expression))
        {
            throw std::runtime_error("word must be in expression.");
        }
        if (std::holds_alternative<r::Literal>(expression))
        {
            throw std::runtime_error("literal must be in expression.");
        }
        r::Operation& operation = std::get<r::Operation>(expression);
        this->ast.push_back(std::move(operation));
        parser.skip_comments_and_spaces();
    }
}

}