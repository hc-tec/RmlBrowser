//
// Created by titto on 2022/12/30.
//

#include "Glue.h"

#include "Dom/Document.h"
#include "Dom/Element.h"
#include "Dom/Event.h"

namespace Rml {

void RegisterGlueFunc(GlueFunc func) {
    GLUE_FUNC_LIST.push_back(func);
}

namespace Script {

void Glue(qjs::Context* context) {
    qjs::Context::Module& dom = context->addModule("dom");
    Event::Glue(dom);
    Element::Glue(dom);
	Document::Glue(dom);
	for (auto func : GLUE_FUNC_LIST) {
		func(context);
	}
	context->eval("import * as dom from 'dom';globalThis.dom = dom;", "<eval>", JS_EVAL_TYPE_MODULE);
}

}

}