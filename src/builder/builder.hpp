// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <break_type.hpp>
#include <local.hpp>
#include <label.hpp>
#include <resolver/resolver.hpp>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>

#include <memory>
#include <map>
#include <cstddef>

namespace r {

struct Binary;
struct Module;
struct ExportGroup;
struct Operation;
struct Type;
struct Procedure;
struct ProcedureGroup;
struct SymbolTable;

// Generates IR using LLVM for local code.
struct Builder final
{
   r::Resolver resolver{};

   // frame
   llvm::SmallVector<std::unique_ptr<r::Local>> locals{};
   llvm::SmallVector<llvm::SmallVector<std::size_t>> scopes{};
   std::map<std::string_view, r::Local*> local_table{};
   llvm::StringMap<r::Label> label_table{};

   llvm::SmallVector<llvm::BasicBlock*> llvm_continue_stack{};
   llvm::SmallVector<llvm::BasicBlock*> llvm_break_stack{};
   std::unique_ptr<llvm::IRBuilder<>> llvm_builder{};
   llvm::BasicBlock* current_block = nullptr;

   // llvm_builder.cpp
   void initialize(r::Binary& builder);
   bool get_is_initialized() const noexcept;
   void terminate() noexcept;

   // module.cpp
   void generate_prototypes(r::Module& module);
   void generate_module(r::Module& module);


private:
   /*
    "generate" functions are used to generate IR with LLVM. There are 4
     main categories of generate functions:

     1. statement functions - generate a statement that does not return
        anything. These are always for top level operations of a scope.
     2. value expression functions - generate an expression that returns
        result as llvm::Value*. These are mainly used for non-aggregate
        types.
     3. store expression functions - generate an expression that stores the
        result in a memory location stored in a location represented by an
        llvm::Value*.
     4. group expressions - generate an expression that performs an
        operation on arguments and returns an llvm::Value*. These are used
        for encapsulating logic for primitive operations between binary and 
        assignment opcodes.

     Some opcodes have different functions bellow associated with them that
     belong to a different category. The appropriate builder function must
     be choosen based on the opcode and expected return. 

     The reason for the separetion between value expressions and store 
     expressions is performance of generated IR:

     https://llvm.org/docs/Frontend/PerformanceTips.html#avoid-creating-values-of-aggregate-type     

     In addition to the above categories, some functions contain the word
     "attribute" in place of "generate". These functions take in attributes and
     resolve them before generating what the attributes are applied to.
   */

   // procedures.cpp
   void generate_prototype(r::Procedure& procedure);
   void generate_body(r::Procedure& procedure);
   void generate_appended_return(r::Procedure& procedure);

   // object.cpp
   void generate_prototype(r::Object& object);
   void generate_property_initializers(r::Procedure& procedure);
   llvm::Value* generate_owner_location(const r::Expression& expression);
   llvm::Value* generate_property_location(r::Object& object, llvm::Value* llvm_object_location, std::string_view name);
   llvm::Value* generate_this_location(const r::Operation& operation);

   // blocks.cpp
   llvm::BasicBlock* create_block(std::string_view name);
   void set_current_block(llvm::BasicBlock* block);

   // statement.cpp
   r::BreakType attribute_statement(const r::Operation& operation);
   r::BreakType generate_statement(const r::Operation& operation);

   // value_expression.cpp
   llvm::Value* attribute_value_expression(const r::Operation& operation, const r::Type& expected_type);
   llvm::Value* generate_value_expression(const r::Expression& expression, const r::Type& expected_type);

   // store_expression.cpp
   void attribute_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);
   void generate_store_expression(const r::Expression& expression, llvm::Value* llvm_store, const r::Type& expected_type);

   // frame.cpp
   void generate_arguments();
   void push_scope();
   void pop_scope();
   void finish_frame();
   r::Local& add_local(std::string_view name, const r::Type& type);
   r::Local& add_temp_local(const r::Type& type);
   r::Local& get_local(std::string_view name);
public:
   r::Local* try_get_local(std::string_view name);
private:
   void generate_local(r::Local& local, llvm::Value* llvm_dynamic_array_size = nullptr);
   llvm::AllocaInst* generate_alloca(llvm::Type* llvm_type, std::string_view name = "temp", llvm::Value* llvm_dynamic_array_size = nullptr);

   // local.cpp
   void generate_local_statement(const r::Operation& operation, const r::Operation* attributes_ptr = nullptr);
   llvm::Value* generate_local_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_local_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);

   // constant.cpp
   llvm::Value* generate_primitive_literal(const r::Literal& literal, bool is_negative, const r::Type& expected_type);
   llvm::ConstantInt* generate_integer_constant(const r::Expression& expression, const r::Type& expected_type);
   llvm::GlobalVariable* generate_global_string(const r::Literal& literal);

   // sign.cpp
   llvm::Value* generate_negative_value_expression(const r::Operation& operation, const r::Type& expected_typepe);
   llvm::Value* generate_positive_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // add.cpp
   llvm::Value* generate_add_group_expression(std::span<const r::Expression> span, const r::Type& expected_type);
   llvm::Value* generate_add_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_add_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_add_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // sub.cpp
   llvm::Value* generate_sub_group_expression(std::span<const r::Expression> span, const r::Type& expected_type);
   llvm::Value* generate_sub_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_sub_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_sub_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // mul.cpp
   llvm::Value* generate_mul_group_expression(std::span<const r::Expression> span, const r::Type& expected_type);
   llvm::Value* generate_mul_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_mul_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_mul_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // div.cpp
   llvm::Value* generate_div_group_expression(std::span<const r::Expression> span, const r::Type& expected_type);
   llvm::Value* generate_div_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_div_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_div_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // rem.cpp
   llvm::Value* generate_rem_group_expression(std::span<const r::Expression> span, const r::Type& expected_type);
   llvm::Value* generate_rem_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_rem_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_rem_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // logical.cpp
   llvm::Value* generate_logical_and_value_expression(const r::Operation& operation, const r::Type& expected_type);
   llvm::Value* generate_logical_or_value_expression(const r::Operation& operation, const r::Type& expected_type);
   llvm::Value* generate_logical_not_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // bool.cpp
   llvm::Constant* generate_true_constant_expression(const r::Operation& operation, const r::Type& expected_type);
   llvm::Constant* generate_false_constant_expression(const r::Operation& operation, const r::Type& expected_type);

   // gt.cpp
   llvm::Value* generate_compare_gt_value_expression(const r::Operation& operation, const r::Type& expected_type);
   llvm::Value* generate_compare_gteq_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // lt.cpp
   llvm::Value* generate_compare_lt_value_expression(const r::Operation& operation, const r::Type& expected_type);
   llvm::Value* generate_compare_lteq_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // eq.cpp
   llvm::Value* generate_compare_eq_value_expression(const r::Operation& operation, const r::Type& expected_type);
   llvm::Value* generate_compare_neq_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // bitwise_and.cpp
   llvm::Value* generate_bitwise_and_group_expression(std::span<const r::Expression> span, const r::Type& expected_type);
   llvm::Value* generate_bitwise_and_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_bitwise_and_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_bitwise_and_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // bitwise_or.cpp
   llvm::Value* generate_bitwise_or_group_expression(std::span<const r::Expression> span, const r::Type& expected_type);
   llvm::Value* generate_bitwise_or_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_bitwise_or_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_bitwise_or_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // bitwise_xor.cpp
   llvm::Value* generate_bitwise_xor_group_expression(std::span<const r::Expression> span, const r::Type& expected_type);
   llvm::Value* generate_bitwise_xor_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_bitwise_xor_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_bitwise_xor_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // bitwise_complement.cpp
   llvm::Value* generate_bitwise_complement_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_bitwise_complement_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_bitwise_complement_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // assignment.cpp
   bool get_is_indeterminate_value(const r::Expression& expression);
   llvm::Value* generate_store_location(const r::Expression& expression, bool is_assigning);
   void generate_value_assignment(llvm::Value* llvm_location, llvm::Value* llvm_value, bool is_volatile = false);
   void generate_assignment_statement(const r::Operation& operation);
   llvm::Value* generate_assignment_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_assignment_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);

   // assertion.cpp
   void generate_assert_statement(const r::Operation& operation);
   void generate_assume_statement(const r::Operation& operation);
   void generate_unreachable_statement(const r::Operation& operation);

   // jump.cpp
   void generate_return_statement(const r::Operation& operation);
   void generate_goto_statement(const r::Operation& operation);
   void generate_break_statement(const r::Operation& operation);
   void generate_continue_statement(const r::Operation& operation);
   void generate_label_statement(const r::Operation& operation);

   // switch.cpp
   void generate_switch_statement(const r::Operation& operation);
   r::BreakType generate_case_statement(const r::Operation& operation, r::BreakType prev_break_type, const r::Type& expected_type, llvm::SwitchInst* llvm_switch);
   void generate_default_case_statement(const r::Operation& operation, r::BreakType prev_break_type, const r::Type& expected_type, llvm::BasicBlock* llvm_default_case_block, llvm::SwitchInst* llvm_switch);
    
   // condition.cpp
   void generate_condition_statement(const r::Operation& operation);

   // ternary.cpp
   void generate_ternary_statement(const r::Operation& operation);
   llvm::Value* generate_ternary_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_ternary_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);

   // infinite_loop.cpp
   void generate_infinite_loop_statement(const r::Operation& operation);

   // for.cpp
   void generate_for_statement(const r::Operation& operation);

   // while.cpp
   void generate_while_statement(const r::Operation& operation);

   // call.cpp
   void generate_call_arguments(r::Procedure& callee, const r::Operation& operation, llvm::SmallVector<llvm::Value*>& llvm_arguments);
   void generate_call_statement(const r::Operation& operation);
   llvm::Value* generate_call_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_call_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);

   // construct.cpp
   void generate_construct_statement(const r::Operation& operation);
   llvm::Value* generate_construct_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_construct_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);

   // destruct.cpp
   void generate_desruct_statement(const r::Operation& operation);
   void generate_destruct(const r::Expression& expression);
   void generate_destruct(r::Local& local);
   void generate_autodestruct_object(r::Object& object, llvm::Value* llvm_object_location);
   void generate_autodestruct_scope();
   void generate_autodestruct_frame();

   // bit_cast.cpp
   llvm::Value* generate_bit_cast_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_bit_cast_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);

   // table.cpp
   void generate_access_table_statement(const r::Operation& operation);
   llvm::Value* generate_access_table_value_expression(const r::Operation& operation, const r::Type& expected_type);
   void generate_access_table_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);

   // member.cpp
   llvm::Value* generate_access_member_location(const r::Operation& operation, bool ignore_last = false);
   llvm::Value* generate_access_member_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // dereference.cpp
   llvm::Value* generate_dereference_location(const r::Operation& operation);
   llvm::Value* generate_dereference_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // index_into.cpp
   llvm::Value* generate_index_into_location(const r::Operation& operation);
   llvm::Value* generate_index_into_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // address_of.cpp
   llvm::Value* generate_address_of_value_expression(const r::Operation& operation, const r::Type& expected_type);

   // variadic_arugments.cpp
   void generate_start_variadic_arguments_statement(const r::Operation& operation);
   void generate_access_variadic_argument_statement(const r::Operation& operation);
   llvm::Value* generate_access_variadic_argument_value_expression(const r::Operation& operation, const r::Type& expected_type);
   llvm::Value* generate_access_variadic_argument_store_expression(const r::Operation& operation, llvm::Value* llvm_store, const r::Type& expected_type);
   void generate_end_variadic_arguments_statement(const r::Operation& operation);
};

}