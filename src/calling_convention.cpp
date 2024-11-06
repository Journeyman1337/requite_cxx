// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <calling_convention.hpp>
#include <utility.hpp>

#include <llvm/IR/CallingConv.h>

#include <string_view>
#include <map>
#include <cassert>
#include <utility>

namespace r {

std::string_view to_string(r::CallingConvention calling_convention) 
{
    switch (calling_convention) 
    {
        case r::CallingConvention::C:
            return "C";
        case r::CallingConvention::FAST:
            return "FAST";
        case r::CallingConvention::COLD:
            return "COLD";
        case r::CallingConvention::GHC:
            return "GHC";
        case r::CallingConvention::HIPE:
            return "HIPE";
        case r::CallingConvention::PRESERVE_MOST:
            return "PRESERVE_MOST";
        case r::CallingConvention::PRESERVE_ALL:
            return "PRESERVE_ALL";
        case r::CallingConvention::SWIFT:
            return "SWIFT";
        case r::CallingConvention::CXX_FAST_TLS:
            return "CXX_FAST_TLS";
        case r::CallingConvention::TAIL:
            return "TAIL";
        case r::CallingConvention::CFGUARD_CHECK:
            return "CFGUARD_CHECK";
        case r::CallingConvention::PRESERVE_NONE:
            return "PRESERVE_NONE";
        case r::CallingConvention::FIRST_TARGET_CC:
            return "FIRST_TARGET_CC";
        case r::CallingConvention::X86_STD_CALL:
            return "X86_STD_CALL";
        case r::CallingConvention::X86_FAST_CALL:
            return "X86_FAST_CALL";
        case r::CallingConvention::ARM_APCS:
            return "ARM_APCS";
        case r::CallingConvention::ARM_AAPCS:
            return "ARM_AAPCS";
        case r::CallingConvention::ARM_AAPCS_VFP:
            return "ARM_AAPCS_VFP";
        case r::CallingConvention::MSP430_INTR:
            return "MSP430_INTR";
        case r::CallingConvention::X96_THIS_CALL:
            return "X96_THIS_CALL";
        case r::CallingConvention::PTX_KERNEL:
            return "PTX_KERNEL";
        case r::CallingConvention::PTX_DEVICE:
            return "PTX_DEVICE";
        case r::CallingConvention::SPIR_FUNC:
            return "SPIR_FUNC";
        case r::CallingConvention::SPIR_KERNEL:
            return "SPIR_KERNEL";
        case r::CallingConvention::INTEL_OCL_BI:
            return "INTEL_OCL_BI";
        case r::CallingConvention::X86_64_SYS_V:
            return "X86_64_SYS_V";
        case r::CallingConvention::WIN64:
            return "WIN64";
        case r::CallingConvention::X86_VECTOR_CALL:
            return "X86_VECTOR_CALL";
        case r::CallingConvention::DUMMY_HHVM:
            return "DUMMY_HHVM";
        case r::CallingConvention::DUMMY_HHVM_C:
            return "DUMMY_HHVM_C";
        case r::CallingConvention::X86_INTR:
            return "X86_INTR";
        case r::CallingConvention::AVR_INTR:
            return "AVR_INTR";
        case r::CallingConvention::AVR_SIGNAL:
            return "AVR_SIGNAL";
        case r::CallingConvention::AVR_BUILTIN:
            return "AVR_BUILTIN";
        case r::CallingConvention::AMDGPU_VS:
            return "AMDGPU_VS";
        case r::CallingConvention::AMDGPU_GS:
            return "AMDGPU_GS";
        case r::CallingConvention::AMDGPU_PS:
            return "AMDGPU_PS";
        case r::CallingConvention::AMDGPU_CS:
            return "AMDGPU_CS";
        case r::CallingConvention::AMDGPU_KERNEL:
            return "AMDGPU_KERNEL";
        case r::CallingConvention::X86_REG_CALL:
            return "X86_REG_CALL";
        case r::CallingConvention::AMDGPU_HS:
            return "AMDGPU_HS";
        case r::CallingConvention::MSP430_BUILTIN:
            return "MSP430_BUILTIN";
        case r::CallingConvention::AMDGPU_LS:
            return "AMDGPU_LS";
        case r::CallingConvention::AMDGPU_ES:
            return "AMDGPU_ES";
        case r::CallingConvention::AARCH64_VECTOR_CALL:
            return "AARCH64_VECTOR_CALL";
        case r::CallingConvention::AARCH64_SVE_VECTOR_CALL:
            return "AARCH64_SVE_VECTOR_CALL";
        case r::CallingConvention::WASM_EMSCRIPTEN_INVOKE:
            return "WASM_EMSCRIPTEN_INVOKE";
        case r::CallingConvention::AMDGPU_GFX:
            return "AMDGPU_GFX";
        case r::CallingConvention::M68K_INTR:
            return "M68K_INTR";
        case r::CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X0:
            return "AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X0";
        case r::CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X2:
            return "AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X2";
        case r::CallingConvention::AMDGPU_CS_CHAIN:
            return "AMDGPU_CS_CHAIN";
        case r::CallingConvention::AMDGPU_CS_CHAIN_PRESERVE:
            return "AMDGPU_CS_CHAIN_PRESERVE";
        case r::CallingConvention::M68K_RTD:
            return "M68K_RTD";
        case r::CallingConvention::GRAAL:
            return "GRAAL";
        case r::CallingConvention::ARM64EC_TUNK_X64:
            return "ARM64EC_TUNK_X64";
        case r::CallingConvention::ARM64EC_THUNK_NATIVE:
            return "ARM64EC_THUNK_NATIVE";
        //case r::CallingConvention::RISCV_VECTOR_CALL:
        //    return "RISCV_VECTOR_CALL";
        //case r::CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X1:
        //    return "AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X1";
        default:
            r::unreachable();
    }    
    r::unreachable();
}

r::CallingConvention to_calling_convention(std::string_view text)
{
    static const std::map<std::string_view, r::CallingConvention> calling_convention_map {
        {"C", r::CallingConvention::C},
        {"FAST", r::CallingConvention::FAST},
        {"COLD", r::CallingConvention::COLD},
        {"GHC", r::CallingConvention::GHC},
        {"HIPE", r::CallingConvention::HIPE},
        {"PRESERVE_MOST", r::CallingConvention::PRESERVE_MOST},
        {"PRESERVE_ALL", r::CallingConvention::PRESERVE_ALL},
        {"SWIFT", r::CallingConvention::SWIFT},
        {"CXX_FAST_TLS", r::CallingConvention::CXX_FAST_TLS},
        {"TAIL", r::CallingConvention::TAIL},
        {"CFGUARD_CHECK", r::CallingConvention::CFGUARD_CHECK},
        {"PRESERVE_NONE", r::CallingConvention::PRESERVE_NONE},
        {"FIRST_TARGET_CC", r::CallingConvention::FIRST_TARGET_CC},
        {"X86_STD_CALL", r::CallingConvention::X86_STD_CALL},
        {"X86_FAST_CALL", r::CallingConvention::X86_FAST_CALL},
        {"ARM_APCS", r::CallingConvention::ARM_APCS},
        {"ARM_AAPCS", r::CallingConvention::ARM_AAPCS},
        {"ARM_AAPCS_VFP", r::CallingConvention::ARM_AAPCS_VFP},
        {"MSP430_INTR", r::CallingConvention::MSP430_INTR},
        {"X96_THIS_CALL", r::CallingConvention::X96_THIS_CALL},
        {"PTX_KERNEL", r::CallingConvention::PTX_KERNEL},
        {"PTX_DEVICE", r::CallingConvention::PTX_DEVICE},
        {"SPIR_FUNC", r::CallingConvention::SPIR_FUNC},
        {"SPIR_KERNEL", r::CallingConvention::SPIR_KERNEL},
        {"INTEL_OCL_BI", r::CallingConvention::INTEL_OCL_BI},
        {"X86_64_SYS_V", r::CallingConvention::X86_64_SYS_V},
        {"WIN64", r::CallingConvention::WIN64},
        {"X86_VECTOR_CALL", r::CallingConvention::X86_VECTOR_CALL},
        {"DUMMY_HHVM", r::CallingConvention::DUMMY_HHVM},
        {"DUMMY_HHVM_C", r::CallingConvention::DUMMY_HHVM_C},
        {"X86_INTR", r::CallingConvention::X86_INTR},
        {"AVR_INTR", r::CallingConvention::AVR_INTR},
        {"AVR_SIGNAL", r::CallingConvention::AVR_SIGNAL},
        {"AVR_BUILTIN", r::CallingConvention::AVR_BUILTIN},
        {"AMDGPU_VS", r::CallingConvention::AMDGPU_VS},
        {"AMDGPU_GS", r::CallingConvention::AMDGPU_GS},
        {"AMDGPU_PS", r::CallingConvention::AMDGPU_PS},
        {"AMDGPU_CS", r::CallingConvention::AMDGPU_CS},
        {"AMDGPU_KERNEL", r::CallingConvention::AMDGPU_KERNEL},
        {"X86_REG_CALL", r::CallingConvention::X86_REG_CALL},
        {"AMDGPU_HS", r::CallingConvention::AMDGPU_HS},
        {"MSP430_BUILTIN", r::CallingConvention::MSP430_BUILTIN},
        {"AMDGPU_LS", r::CallingConvention::AMDGPU_LS},
        {"AMDGPU_ES", r::CallingConvention::AMDGPU_ES},
        {"AARCH64_VECTOR_CALL", r::CallingConvention::AARCH64_VECTOR_CALL},
        {"AARCH64_SVE_VECTOR_CALL", r::CallingConvention::AARCH64_SVE_VECTOR_CALL},
        {"WASM_EMSCRIPTEN_INVOKE", r::CallingConvention::WASM_EMSCRIPTEN_INVOKE},
        {"AMDGPU_GFX", r::CallingConvention::AMDGPU_GFX},
        {"M68K_INTR", r::CallingConvention::M68K_INTR},
        {"AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X0", r::CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X0},
        {"AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X2", r::CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X2},
        {"AMDGPU_CS_CHAIN", r::CallingConvention::AMDGPU_CS_CHAIN},
        {"AMDGPU_CS_CHAIN_PRESERVE", r::CallingConvention::AMDGPU_CS_CHAIN_PRESERVE},
        {"M68K_RTD", r::CallingConvention::M68K_RTD},
        {"GRAAL", r::CallingConvention::GRAAL},
        {"ARM64EC_TUNK_X64", r::CallingConvention::ARM64EC_TUNK_X64},
        {"ARM64EC_THUNK_NATIVE", r::CallingConvention::ARM64EC_THUNK_NATIVE},
        //{"RISCV_VECTOR_CALL", r::CallingConvention::RISCV_VECTOR_CALL},
        //{"AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X1", r::CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X1}
    };

    auto it = calling_convention_map.find(text);
    if (it != calling_convention_map.end())
    {
        return it->second;
    }
    else
    {
        return r::CallingConvention::UNKNOWN;
    }
}

llvm::CallingConv::ID to_llvm_call_conv(CallingConvention calling_convention)
{
    switch (calling_convention)
    {
        case CallingConvention::C:
            return llvm::CallingConv::C;
        case CallingConvention::FAST:
            return llvm::CallingConv::Fast;
        case CallingConvention::COLD:
            return llvm::CallingConv::Cold;
        case CallingConvention::GHC:
            return llvm::CallingConv::GHC;
        case CallingConvention::HIPE:
            return llvm::CallingConv::HiPE;
        case CallingConvention::PRESERVE_MOST:
            return llvm::CallingConv::PreserveMost;
        case CallingConvention::PRESERVE_ALL:
            return llvm::CallingConv::PreserveAll;
        case CallingConvention::SWIFT:
            return llvm::CallingConv::Swift;
        case CallingConvention::CXX_FAST_TLS:
            return llvm::CallingConv::X86_64_SysV;
        case CallingConvention::TAIL:
            return llvm::CallingConv::Tail;
        case CallingConvention::CFGUARD_CHECK:
            return llvm::CallingConv::X86_StdCall;
        case CallingConvention::PRESERVE_NONE:
            return llvm::CallingConv::X86_FastCall;
        case CallingConvention::FIRST_TARGET_CC:
            return llvm::CallingConv::FirstTargetCC;
        case CallingConvention::X86_STD_CALL:
            return llvm::CallingConv::X86_StdCall;
        case CallingConvention::X86_FAST_CALL:
            return llvm::CallingConv::X86_FastCall;
        case CallingConvention::ARM_APCS:
            return llvm::CallingConv::ARM_APCS;
        case CallingConvention::ARM_AAPCS:
            return llvm::CallingConv::ARM_AAPCS;
        case CallingConvention::ARM_AAPCS_VFP:
            return llvm::CallingConv::ARM_AAPCS_VFP;
        case CallingConvention::MSP430_INTR:
            return llvm::CallingConv::MSP430_INTR;
        case CallingConvention::X96_THIS_CALL:
            return llvm::CallingConv::X86_ThisCall;
        case CallingConvention::PTX_KERNEL:
            return llvm::CallingConv::PTX_Kernel;
        case CallingConvention::PTX_DEVICE:
            return llvm::CallingConv::PTX_Device;
        case CallingConvention::SPIR_FUNC:
            return llvm::CallingConv::SPIR_FUNC;
        case CallingConvention::SPIR_KERNEL:
            return llvm::CallingConv::SPIR_KERNEL;
        case CallingConvention::INTEL_OCL_BI:
            return llvm::CallingConv::Intel_OCL_BI;
        case CallingConvention::X86_64_SYS_V:
            return llvm::CallingConv::X86_64_SysV;
        case CallingConvention::WIN64:
            return llvm::CallingConv::Win64;
        case CallingConvention::X86_VECTOR_CALL:
            return llvm::CallingConv::X86_VectorCall;
        case CallingConvention::DUMMY_HHVM:
            return llvm::CallingConv::DUMMY_HHVM;
        case CallingConvention::DUMMY_HHVM_C:
            return llvm::CallingConv::DUMMY_HHVM_C;
        case CallingConvention::X86_INTR:
            return llvm::CallingConv::X86_INTR;
        case CallingConvention::AVR_INTR:
            return llvm::CallingConv::AVR_INTR;
        case CallingConvention::AVR_SIGNAL:
            return llvm::CallingConv::AVR_SIGNAL;
        case CallingConvention::AVR_BUILTIN:
            return llvm::CallingConv::AVR_BUILTIN;
        case CallingConvention::AMDGPU_VS:
            return llvm::CallingConv::AMDGPU_VS;
        case CallingConvention::AMDGPU_GS:
            return llvm::CallingConv::AMDGPU_GS;
        case CallingConvention::AMDGPU_PS:
            return llvm::CallingConv::AMDGPU_PS;
        case CallingConvention::AMDGPU_CS:
            return llvm::CallingConv::AMDGPU_CS;
        case CallingConvention::AMDGPU_KERNEL:
            return llvm::CallingConv::AMDGPU_KERNEL;
        case CallingConvention::X86_REG_CALL:
            return llvm::CallingConv::X86_RegCall;
        case CallingConvention::AMDGPU_HS:
            return llvm::CallingConv::AMDGPU_HS;
        case CallingConvention::MSP430_BUILTIN:
            return llvm::CallingConv::MSP430_BUILTIN;
        case CallingConvention::AMDGPU_LS:
            return llvm::CallingConv::AMDGPU_LS;
        case CallingConvention::AMDGPU_ES:
            return llvm::CallingConv::AMDGPU_ES;
        case CallingConvention::AARCH64_VECTOR_CALL:
            return llvm::CallingConv::AArch64_VectorCall;
        case CallingConvention::AARCH64_SVE_VECTOR_CALL:
            return llvm::CallingConv::AArch64_SVE_VectorCall;
        case CallingConvention::WASM_EMSCRIPTEN_INVOKE:
            return llvm::CallingConv::WASM_EmscriptenInvoke;
        case CallingConvention::AMDGPU_GFX:
            return llvm::CallingConv::AMDGPU_Gfx;
        case CallingConvention::M68K_INTR:
            return llvm::CallingConv::M68k_INTR;
        case CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X0:
            return llvm::CallingConv::AArch64_SME_ABI_Support_Routines_PreserveMost_From_X0;
        case CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X2:
            return llvm::CallingConv::AArch64_SME_ABI_Support_Routines_PreserveMost_From_X2;
        case CallingConvention::AMDGPU_CS_CHAIN:
            return llvm::CallingConv::AMDGPU_CS;
        case CallingConvention::AMDGPU_CS_CHAIN_PRESERVE:
            return llvm::CallingConv::AMDGPU_CS_ChainPreserve;
        case CallingConvention::M68K_RTD:
            return llvm::CallingConv::M68k_RTD;
        case CallingConvention::GRAAL:
            return llvm::CallingConv::GRAAL;
        case CallingConvention::ARM64EC_TUNK_X64:
            return llvm::CallingConv::ARM64EC_Thunk_X64;
        case CallingConvention::ARM64EC_THUNK_NATIVE:
            return llvm::CallingConv::ARM64EC_Thunk_Native;
        //case CallingConvention::RISCV_VECTOR_CALL:
        //    return llvm::CallingConv::RISCV_VectorCall;
        //case CallingConvention::AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X1:
        //    return llvm::CallingConv::AArch64_SME_ABI_Support_Routines_PreserveMost_From_X1;
        default:
            r::unreachable();
    }
}

}