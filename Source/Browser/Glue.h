//
// Created by titto on 2023/1/10.
//

#ifndef RMLUI_BROWSER_GLUE_H
#define RMLUI_BROWSER_GLUE_H

#include "../Script/Glue.h"
#include "Tab.h"
#include "RmlUi/Core/ElementDocument.h"
#include "MainWindow.h"

namespace Rml {

namespace Browser {

static void TabGlue(qjs::Context* context) {
//	context->registerClass<Tab>("Tab");


    auto& m = context->addModule("browser");
	m.class_<Tab>("Tab")
	    .fun<&Tab::document>("getDocument")
        .fun<&Tab::tab_id>("getId");
    context->eval("import * as browser from 'browser';globalThis.browser = browser;", "<eval>", JS_EVAL_TYPE_MODULE);

    context->global()["COpenTabWithUrl"] = [&](const Rml::String& tab_id, const Rml::String& url){
      auto win = Rml::Browser::MainWindow::GetInstance();
      if (tab_id == "-1") {
          win->DoTabOpenNew(url);
      } else
      {
          win->DoTabEnterUrl(tab_id, url);
      }
    };

}

static void RegisterBrowserGlueFunc() {
    RegisterGlueFunc(TabGlue);
}

}

}

#endif // RMLUI_BROWSER_GLUE_H
