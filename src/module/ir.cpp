// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <module/module.hpp>
#include <binary.hpp>
#include <procedure.hpp>
#include <builder/builder.hpp>
#include <file_io.hpp>

#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

#include <memory>
#include <string>
#include <filesystem>
#include <format>

namespace r {

void Module::initialize_llvm_module()
{
    this->llvm_module = std::make_unique<llvm::Module>(this->mangled_name.c_str(), *this->binary->llvm_context.get());
    this->llvm_module->setSourceFileName(this->path.c_str());
}

void Module::write_ir_file()
{
    std::string ir_source;
    llvm::raw_string_ostream raw_string_ostream(ir_source);
    this->llvm_module->print(raw_string_ostream, nullptr);
    std::filesystem::path ir_file_path = this->path.replace_filename(std::format("{}.ir", this->mangled_name));
    r::write_file_text(ir_file_path, ir_source);
}

}
