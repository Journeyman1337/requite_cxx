// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <procedure.hpp>

#include <cassert>

namespace r {

bool Procedure::get_has_body() const noexcept
{
    assert(this->declaration != nullptr);
    return this->body_start_i < this->declaration->branches.size();
}

bool Procedure::get_is_instanced() const noexcept
{
    return r::get_is_instanced(this->category);
}

bool Procedure::get_has_sret() const noexcept
{
    return this->return_type.get_is_llvm_store_type();
}

bool Procedure::get_is_constructor() const noexcept
{
    return r::get_is_constructor(this->category);
}

bool Procedure::get_is_destructor() const noexcept
{
    return r::get_is_destructor(this->category);
}

bool Procedure::get_is_default() const noexcept
{
    return r::get_is_default(this->category);
}

llvm::Value* Procedure::get_this()
{
    assert(this->get_is_instanced());
    auto arg_iter = this->llvm_function->arg_begin();
    if (
        this->get_is_constructor() ||
        this->get_is_destructor()
    )
    {
        return arg_iter;
    }
    if (this->get_has_sret())
    {
        arg_iter++;
    }
    return arg_iter;
}

llvm::Value* Procedure::get_sret()
{
    assert(this->get_has_sret());
    auto arg_iter = this->llvm_function->arg_begin();
    return arg_iter;
}

r::Type Procedure::get_sret_type() const noexcept
{
    assert(this->get_has_sret());
    r::Type sret_type = this->return_type;
    sret_type.add_pointer();
    return sret_type;
}

r::Type Procedure::get_this_type() const noexcept
{
    assert(this->get_is_instanced());
    r::Type this_type = r::Type(this->object);
    this_type.add_pointer();
    return this_type;
}

}