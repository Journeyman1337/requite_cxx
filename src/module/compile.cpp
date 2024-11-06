// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <module/module.hpp>

#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include "llvm/IR/LegacyPassManager.h"

#include <filesystem>
#include <format>

namespace r {

void Module::compile_intermediate_file()
{
    auto target_triple = llvm::sys::getDefaultTargetTriple();
    this->llvm_module->setTargetTriple(target_triple);
    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);
    if (target == nullptr)
    {
        throw std::runtime_error(error);
    }
    const char* cpu = "generic";
    const char* features = "";
    llvm::TargetOptions options;
    auto machine = target->createTargetMachine(target_triple, cpu, features, options, llvm::Reloc::PIC_);
    auto data_layout = machine->createDataLayout();
    this->llvm_module->setDataLayout(data_layout);
    this->llvm_module->setTargetTriple(target_triple);
    std::error_code error_code;
    std::filesystem::path obj_path = this->path.replace_filename(std::format("{}.obj", this->mangled_name));
    llvm::raw_fd_ostream ofile(obj_path.c_str(), error_code, llvm::sys::fs::OF_None);
    if (error_code)
    {
        throw std::runtime_error(error_code.message());
    }
    llvm::legacy::PassManager pass;
    const auto file_type = llvm::CodeGenFileType::ObjectFile;
    if (machine->addPassesToEmitFile(pass, ofile, nullptr, file_type))
    {
        throw std::runtime_error("the target machine can not emit a file of this type.");
    }
    pass.run(*this->llvm_module.get());
    ofile.flush();
}

}