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
		.fun<&ElementDocument::SetTitle>("setTitle")
        .fun<&ElementDocument::GetTitle>("getTitle")
        .fun<&ElementDocument::GetSourceURL>("getSourceURL")
        .fun<>("createElement", [](ElementDocument* _this, const String& tag) {
			return _this->CreateElement(tag).get();
		})
//        .fun_ptr<&ElementDocument::CreateElement>("createElementB")
//        .fun<&ElementDocument::CreateTextNode>("createTextNode")
        .fun<&ElementDocument::GetElementById>("getElementById");
}

}

}