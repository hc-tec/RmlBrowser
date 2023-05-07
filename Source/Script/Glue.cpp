//
// Created by titto on 2022/12/30.
//

#include "Glue.h"

#include <iostream>

#include "Dom/Document.h"
#include "Dom/Element.h"
#include "Dom/Event.h"

#include "Net/Glue.h"

#include "log/logging.h"

namespace Rml {

void RegisterGlueFunc(GlueFunc func) {
    GLUE_FUNC_LIST.push_back(func);
}

namespace Script {

void Glue(qjs::Context* context) {
    context->global()["log"] = [](const Rml::String& str){
      std::cout << str << std::endl;
    };

    qjs::Context::Module& dom = context->addModule("dom");
    Event::Glue(dom);
    Element::Glue(dom);
	Document::Glue(dom);

    NetGlue(context);

    try
    {
        for (auto func : GLUE_FUNC_LIST) {
            func(context);
        }
    }catch (qjs::exception e) {
//        auto exc = e.get();
//        Log::Message(Log::LT_DEBUG, "%s", ((std::string) exc).data());
//        if((bool) exc["stack"])
//            Log::Message(Log::LT_DEBUG, "%s", ((std::string) exc["stack"]).data());
    }
	context->eval("import * as dom from 'dom';import * as net from 'net';"
				        "globalThis.dom = dom;globalThis.net = net;", "<eval>", JS_EVAL_TYPE_MODULE);
}

}

}