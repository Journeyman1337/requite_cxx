// SPDX-FileCopyrightText: 2024 Daniel Aimé Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <builder/builder.hpp>
#include <module/module.hpp>

namespace r {

void Builder::generate_prototypes(r::Module& module)
{
    for (std::unique_ptr<r::Object>& object_ptr : module.objects)
    {
        assert(object_ptr.get() != nullptr);
        r::Object& object = *object_ptr.get();
        this->generate_prototype(object);
    }
    for (std::unique_ptr<r::Procedure>& procedure_ptr : module.procedures)
    {
        assert(procedure_ptr.get() != nullptr);
        r::Procedure& procedure = *procedure_ptr.get();
        this->generate_prototype(procedure);
    }
}

void Builder::generate_module(r::Module& module)
{
    for (std::unique_ptr<r::Procedure>& procedure_ptr : module.procedures)
    {
        r::Procedure& procedure = *procedure_ptr.get();
        this->generate_body(procedure);
    }
}

}

