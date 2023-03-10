//
// Created by titto on 2023/1/10.
//

#ifndef RMLUI_BROWSER_GLUE_H
#define RMLUI_BROWSER_GLUE_H

#include "../Script/Glue.h"
#include "Tab.h"
#include "RmlUi/Core/ElementDocument.h"

namespace Rml {

namespace Browser {

static void TabGlue(qjs::Context* context) {
//	context->registerClass<Tab>("Tab");
    auto& m = context->addModule("browser");
	m.class_<Tab>("Tab")
	    .fun<&Tab::document>("getDocument");
    context->eval("import * as browser from 'browser';globalThis.browser = browser;", "<eval>", JS_EVAL_TYPE_MODULE);
}

static void RegisterBrowserGlueFunc() {
    RegisterGlueFunc(TabGlue);
}

}

}

#endif // RMLUI_BROWSER_GLUE_H
