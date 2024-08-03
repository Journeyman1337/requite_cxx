// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <expression.hpp>
#include <symbol.hpp>
#include <special_type.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>

#include <cstddef>
#include <span>

namespace r {

struct Binary;
struct Module;
struct ExecutableModule;
struct ExportGroup;
struct Object;
struct ObjectExtension;
struct Procedure;
struct Property;
struct ProcedureGroup;
struct SymbolTable;
struct Type;
struct Builder;
struct Integer;
struct Codeunit;
struct FloatingPoint;
struct FixedPoint;

// Tracks the current global scope being processed and performs type operations
// such as resolution and deduction.
struct Resolver final
{
   r::Binary* binary = nullptr;
   r::Module* module = nullptr;
   r::ExportGroup* export_group = nullptr;
   r::Object* object = nullptr;
   r::Procedure* procedure = nullptr;
   r::SymbolTable* table = nullptr;

   // location.cpp
   void enter(r::Binary& binary);
   void enter(r::Module& module);
   void enter(r::ExportGroup& export_group, r::Module& module);
   void enter(r::Object& object);
   void enter(r::Procedure& procedure);
   void enter(r::TypeAlias& type_alias);
   void clear();

   // types.cpp
   bool get_is_type_assignable_to_type(const r::Type& from, const r::Type& to);
   void check_type_assignable_to_type(const r::Type& from, const r::Type& to);
   void check_dereferenced_type_assignable_to_type(const r::Type& from, const r::Type& to);
   void check_indexed_type_assignable_to_type(const r::Type& from, const r::Type& to);
   std::size_t get_bit_depth(const r::Type& type);
   std::size_t get_byte_size(const r::Type& type);
   r::Type resolve_type(const r::Expression& expression, bool can_fail = false);
   r::Type deduce_type(const r::Expression& expression, r::Builder* builder = nullptr);
   r::Type deduce_group_type(std::span<const r::Expression> branch_group, r::Builder* builder = nullptr);
   r::Type deduce_group_type(const r::Type& type_a, const r::Type& type_b);

   // llvm.cpp
   llvm::Function* get_llvm_function() const noexcept;
   llvm::Module& get_llvm_module() const noexcept;
   llvm::LLVMContext& get_llvm_context() const noexcept;
   llvm::Type* get_llvm_size_type() const;
   llvm::IntegerType* get_llvm_type(const r::Integer& integer);
   llvm::IntegerType* get_llvm_type(const r::Codeunit& codeunit);
   llvm::Type* get_llvm_type(r::SpecialType special_type);
   llvm::Type* get_llvm_type(const r::FloatingPoint& floating_point);
   llvm::Type* get_llvm_type(const r::FixedPoint& fixed_point);
   llvm::Type* get_llvm_type(r::Object& object);
   llvm::Type* get_llvm_type(const r::Type& type, std::size_t dropped_subtypes = 0UZ);
   std::size_t get_pointer_bit_depth() const noexcept;

   // procedures.cpp
   r::ProcedureGroup& get_procedure_group(const r::Expression& expression, r::Builder* builder = nullptr);
   r::Procedure& get_call_procedure(const r::Operation& call_operation, r::Builder* builder = nullptr);
   r::Procedure& get_construct_procedure(const r::Operation& construct_operation, const r::Type& expected_type, r::Builder* builder = nullptr);
   r::Procedure& get_best_overload(r::ProcedureGroup& procedure_group, std::span<const r::Type> arguments);
   void check_overload_is_unique(r::ProcedureGroup& procedure_group, const r::Procedure& overload) const;

   // table.cpp
   void access_table(const r::Operation& operation, bool ignore_last);
   const r::Expression& resolve_table(const r::Operation& operation);
   void reset_table();
   r::SymbolTable& get_current_table(); // get the current table for adding things while tabulating.
   r::ExportGroup& get_created_export_group(const r::Expression& expression);
   r::Symbol* try_get_symbol(const r::Expression& expression);
   r::Symbol* try_get_symbol(std::string_view name);
   void add_to_table(r::Procedure& procedure);
   void add_to_table(r::Global& global); 
   void add_to_table(r::Object& object);
   void add_to_table(r::ExportGroup& export_group);
   void add_to_table(r::TypeAlias& type_alias);

   // symbols.cpp
   r::ExportGroup& add_export_group();
   r::Procedure& add_procedure();
   r::Global& add_global();
   r::Object& add_object();
   r::ObjectExtension& add_object_extension();
   r::ProcedureGroup& add_procedure_group();
   r::Property& add_property();
   r::TypeAlias& add_type_alias();
};

}