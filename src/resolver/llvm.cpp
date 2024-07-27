// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <resolver/resolver.hpp>
#include <binary.hpp>
#include <module/module.hpp>
#include <procedure.hpp>
#include <utility.hpp>
#include <llvm_extensions.hpp>
#include <type_alias.hpp>
#include <object.hpp>

#include <llvm/IR/Type.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/TargetParser/Host.h>

#include <string>
#include <memory>
#include <ranges>

namespace r {

llvm::Function* Resolver::get_llvm_function() const noexcept
{
    return this->procedure->llvm_function;
}

llvm::Module& Resolver::get_llvm_module() const noexcept
{
    return *this->module->llvm_module.get();
}

llvm::LLVMContext& Resolver::get_llvm_context() const noexcept
{
    return *this->binary->llvm_context.get();
}

llvm::Type* Resolver::get_llvm_size_type() const
{
    return
        this->binary->llvm_data_layout->getIntPtrType(
            this->get_llvm_context(),
            0
        );
}

llvm::IntegerType* Resolver::get_llvm_type(const r::Integer& integer)
{
    return 
        llvm::Type::getIntNTy(
            this->get_llvm_context(),
            integer.bit_depth
        );
}

llvm::IntegerType* Resolver::get_llvm_type(const r::Codeunit& codeunit)
{
    return 
        llvm::Type::getIntNTy(
            this->get_llvm_context(),
            codeunit.get_bit_depth()
        );
}

llvm::Type* Resolver::get_llvm_type(r::SpecialType special_type)
{
    switch (special_type)
    {
        case r::SpecialType::BOOL:
            return 
                llvm::Type::getInt1Ty(
                    this->get_llvm_context()
                );
        case r::SpecialType::BYTE:
            return 
                llvm::Type::getInt8Ty(
                    this->get_llvm_context()
                );
        case r::SpecialType::VARIADIC_ARGUMENTS:
            return 
                llvm::ArrayType::get(
                        llvm::Type::getInt8Ty(
                            this->get_llvm_context()
                        ),
                        24UZ
                    );
        case r::SpecialType::VOID:
            return
                llvm::Type::getVoidTy(this->get_llvm_context());
    }
    r::unreachable();
}

llvm::Type* Resolver::get_llvm_type(const r::FloatingPoint& floating_point)
{
    return
        llvm::Type::getFloatingPointTy(
            this->get_llvm_context(),
            r::get_float_semantics(floating_point.type)
        );
}

llvm::Type* Resolver::get_llvm_type(const r::FixedPoint& fixed_point)
{
    // TODO
    return nullptr;
}

llvm::Type* Resolver::get_llvm_type(r::Object& object)
{
    return object.llvm_struct_type;
}

llvm::Type* Resolver::get_llvm_type(const r::Type& type, std::size_t dropped_subtypes)
{
    assert(dropped_subtypes <= type.subtypes.size());
    llvm::Type* llvm_type = nullptr;
    if (std::holds_alternative<r::Codeunit>(type.root))
    {
        const r::Codeunit& codeunit = std::get<r::Codeunit>(type.root);
        llvm_type = this->get_llvm_type(codeunit);
    }
    else if (std::holds_alternative<r::Integer>(type.root))
    {
        const r::Integer& integer = std::get<r::Integer>(type.root);
        llvm_type = this->get_llvm_type(integer);
    }
    else if (std::holds_alternative<r::FloatingPoint>(type.root))
    {
        const r::FloatingPoint& floating_point = std::get<r::FloatingPoint>(type.root);
        llvm_type = this->get_llvm_type(floating_point);
    }
    else if (std::holds_alternative<r::SpecialType>(type.root))
    {
        r::SpecialType special_type = std::get<r::SpecialType>(type.root);
        llvm_type = this->get_llvm_type(special_type);
    }
    else if (std::holds_alternative<r::Object*>(type.root))
    {
        r::Object& object = *std::get<r::Object*>(type.root);
        llvm_type = this->get_llvm_type(object); 
    }
    assert(!std::holds_alternative<r::TypeAlias*>(type.root));
    assert(llvm_type != nullptr);
    for (const r::Subtype& subtype : type.subtypes | std::views::drop(dropped_subtypes))
    {
        if (subtype.qualifiers.test(r::QualifierFlag::POINTER))
        {
            llvm_type = 
                llvm::PointerType::get(
                    llvm_type,
                    0
                );
        }
        else if (subtype.qualifiers.test(r::QualifierFlag::ARRAY))
        {
            if (subtype.array_size > 0UZ)
            {
                llvm_type =
                    llvm::ArrayType::get(
                        llvm_type,
                        subtype.array_size
                    );
                llvm_type =
                    llvm::StructType::create(
                        this->get_llvm_context(),
                        { llvm_type },
                        "array"
                    );
            }
        }
    }
    return llvm_type;
}

std::size_t Resolver::get_pointer_bit_depth() const noexcept
{
    assert(this->binary->llvm_data_layout != nullptr);
    return this->binary->llvm_data_layout->getPointerSize() * 8UZ;
}


}