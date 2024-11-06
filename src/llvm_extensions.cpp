// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <llvm_extensions.hpp>
#include <utility.hpp>
#include <floating_point.hpp>
#include <floating_point_type.hpp>

#include <llvm/Support/TargetSelect.h>
#include <llvm/IR/Constants.h>

#include <utility>
#include <functional>
#include <cassert>

namespace r {

void initialize_llvm()
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
}

const llvm::fltSemantics& get_float_semantics(const r::FloatingPoint& floating_point)
{
    switch (floating_point.type)
    {
        case r::FloatingPointType::UNKNOWN:
            r::unreachable();
        case r::FloatingPointType::BRAIN:
            return llvm::APFloatBase::BFloat();
        case r::FloatingPointType::BINARY_HALF:
            return llvm::APFloatBase::IEEEhalf();
        case r::FloatingPointType::BINARY_SINGLE:
            return llvm::APFloatBase::IEEEsingle();
        case r::FloatingPointType::BINARY_DOUBLE:
            return llvm::APFloatBase::IEEEdouble();
        case r::FloatingPointType::BINARY_QUAD:
            return llvm::APFloatBase::IEEEquad();
        case r::FloatingPointType::PCC_DOUBLE_DOUBLE:
            return llvm::APFloatBase::PPCDoubleDouble();
        case r::FloatingPointType::FLOAT8_E5M2:
            return llvm::APFloatBase::Float8E5M2();
        case r::FloatingPointType::FLOAT8_E6M2FNUZ:
            return llvm::APFloatBase::Float8E5M2FNUZ();
        case r::FloatingPointType::FLOAT8_E4M3FN:
            return llvm::APFloatBase::Float8E4M3FN();
        case r::FloatingPointType::FLOAT8_E4M3FNUZ:
            return llvm::APFloatBase::Float8E4M3FNUZ();
        case r::FloatingPointType::FLOAT8_E4M3B11FNUZ:
            return llvm::APFloatBase::Float8E4M3B11FNUZ();
        case r::FloatingPointType::FLOAT_TF32:
            return llvm::APFloatBase::FloatTF32();
        case r::FloatingPointType::X87_DOUBLE_EXTENDED:
            return llvm::APFloatBase::x87DoubleExtended();
    }
    r::unreachable();
}

}