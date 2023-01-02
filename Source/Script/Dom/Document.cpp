//
// Created by titto on 2022/12/30.
//

#include "Document.h"

#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/ElementDocument.h"
#include "Ownership.h"

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
        .fun<>("createElement", [](ElementDocument* _this, const String& tag) {
            ElementPtr ele_ptr = _this->CreateElement(tag);
            Element* ele = GetOwnership<Element, ElementPtr>()->ShiftOwner(std::move(ele_ptr));
            return ele;
		});
}

}

}