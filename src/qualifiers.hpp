// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/ADT/Bitset.h>

#include <cstddef>

namespace r {

namespace QualifierFlag {

enum Type : std::size_t
{
    // this is a pointer.
    POINTER,
    // this is an array.
    ARRAY,
    // if the value stored in this type can be changed.
    MUTABLE,
    // if the memory of this type is volatile.
    VOLATILE,
    // used in type deduction only. never found on subtypes.
    LITERAL,
    COUNT
};

}

using QualifierFlagSet = llvm::Bitset<r::QualifierFlag::COUNT>;

}