//
// Created by titto on 2022/12/30.
//

#include "Document.h"

#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/ElementDocument.h"

namespace Rml {

namespace Script {

void Document::Glue(qjs::Context::Module& m)
{
    m.class_<ElementDocument>("Document")
        .constructor<const String&>("Document")
        .base<Rml::Element>()
		.fun<&ElementDocument::SetTitle>("setTitle")
        .fun<&ElementDocument::GetTitle>("getTitle")
        .fun<&ElementDocument::GetSourceURL>("getSourceURL")
        .fun<&ElementDocument::CreateElement>("createElement")
        .fun<&ElementDocument::Hide>("hide")
        .fun<&ElementDocument::Show>("show");
}

}

}