// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#pragma once

#include <variant>

namespace r {

struct Global;
struct Object;
struct ExportGroup;
struct ProcedureGroup;
struct TypeAlias;

using Symbol = std::variant<r::ProcedureGroup*, r::Global*, r::Object*, r::ExportGroup*, r::TypeAlias*>;

}