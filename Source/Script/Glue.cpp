//
// Created by titto on 2022/12/30.
//

#include "Glue.h"

#include "Dom/Document.h"
#include "Dom/Element.h"
#include "Dom/Event.h"

namespace Rml {

namespace Script {

void Glue(qjs::Context* context) {
    qjs::Context::Module& dom = context->addModule("dom");
    Event::Glue(dom);
    Element::Glue(dom);
	Document::Glue(dom);
}

}

}