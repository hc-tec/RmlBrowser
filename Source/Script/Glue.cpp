//
// Created by titto on 2022/12/30.
//

#include "Glue.h"

#include "Dom/Document.h"
#include "Dom/Element.h"

namespace Rml {

namespace Script {

void Glue() {
    qjs::Context* js_context_ = GetContext();

    qjs::Context::Module& dom = js_context_->addModule("dom");
	Document::Glue(dom);
	Element::Glue(dom);
}

}

}