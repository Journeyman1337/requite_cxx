// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <llvm/IR/CallingConv.h>

#include <string_view>

namespace r {

enum class CallingConvention
{
    UNKNOWN,
    C,
    FAST,
    COLD,
    GHC,
    HIPE,
    // ANY_REG,
    PRESERVE_MOST,
    PRESERVE_ALL,
    SWIFT,
    CXX_FAST_TLS,
    TAIL,
    CFGUARD_CHECK,
    PRESERVE_NONE,
    FIRST_TARGET_CC,
    X86_STD_CALL,
    X86_FAST_CALL,
    ARM_APCS,
    ARM_AAPCS,
    ARM_AAPCS_VFP,
    MSP430_INTR,
    X96_THIS_CALL,
    PTX_KERNEL,
    PTX_DEVICE,
    SPIR_FUNC,
    SPIR_KERNEL,
    INTEL_OCL_BI,
    X86_64_SYS_V,
    WIN64,
    X86_VECTOR_CALL,
    DUMMY_HHVM,
    DUMMY_HHVM_C,
    X86_INTR,
    AVR_INTR,
    AVR_SIGNAL,
    AVR_BUILTIN,
    AMDGPU_VS,
    AMDGPU_GS,
    AMDGPU_PS,
    AMDGPU_CS,
    AMDGPU_KERNEL,
    X86_REG_CALL,
    AMDGPU_HS,
    MSP430_BUILTIN,
    AMDGPU_LS,
    AMDGPU_ES,
    AARCH64_VECTOR_CALL,
    AARCH64_SVE_VECTOR_CALL,
    WASM_EMSCRIPTEN_INVOKE,
    AMDGPU_GFX,
    M68K_INTR,
    AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X0,
    AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X2,
    AMDGPU_CS_CHAIN,
    AMDGPU_CS_CHAIN_PRESERVE,
    M68K_RTD,
    GRAAL,
    ARM64EC_TUNK_X64,
    ARM64EC_THUNK_NATIVE,
    //RISCV_VECTOR_CALL,
    //AARCH64_SME_ABI_SUPPORT_ROUTINES_PRESERVE_MOST_FROM_X1
};

std::string_view to_string(r::CallingConvention calling_convention);

r::CallingConvention to_calling_convention(std::string_view name);

llvm::CallingConv::ID to_llvm_call_conv(r::CallingConvention calling_convention);

}