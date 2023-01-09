//
// Created by titto on 2022/12/30.
//

#include "ScriptPlugin.h"

#include <iostream>

#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/Factory.h"
#include "RmlUi/Core/ElementDocument.h"

#include <co/co/mutex.h>
#include "Glue.h"
#include "Dom/JsDocumentElementInstancer.h"
#include "Dom/XMLNodeHandlerAnchor.h"

namespace Rml {

namespace Script {

ScriptPlugin::ScriptPlugin(Context* context)
		: context_(context),
		js_runtime_(nullptr) {
//    static co::mutex js_mutex;
//    auto _ = co::mutex_guard(js_mutex);
    js_runtime_ = GetRunTime(); // new qjs::Runtime();
    js_context_ = MakeUnique<qjs::Context>(js_runtime_->rt);
    Glue(js_context_.get());
    js_document_element_instancer_ = MakeShared<JsDocumentElementInstancer>();
    Factory::RegisterElementInstancer("body", js_document_element_instancer_.get());
    XMLParser::RegisterNodeHandler("a", MakeShared<XMLNodeHandlerAnchor>());
    XMLParser::RegisterNodeHandler("script", js_document_element_instancer_);
}

int ScriptPlugin::GetEventClasses() { return Plugin::GetEventClasses(); }

void ScriptPlugin::OnInitialise() {
    std::cout << "Register ScriptPlugin" << std::endl;
}

void ScriptPlugin::OnShutdown() { Plugin::OnShutdown(); }

void ScriptPlugin::OnContextCreate(Context* context) {
//    context_ = context;

}

void ScriptPlugin::OnContextDestroy(Context* context) {
    if (context != context_) return;
}

void ScriptPlugin::OnDocumentOpen(Context* context, const String& document_path) {
    if (context != context_) return;
}

void ScriptPlugin::OnDocumentLoad(ElementDocument* document) {
	if (document->GetContext() != context_) return;
    qjs::Context* js_context = js_context_.get();
    js_context->global()["document"] = document;
	DocumentHeader::ResourceList scripts = js_document_element_instancer_->GetScripts();
    try {
//        static co::mutex js_mutex;
//        auto _ = co::mutex_guard(js_mutex);
        for (auto& script : scripts) {
            if (script.is_inline) {
                js_context->eval(script.content, script.path.data());
            } else {
				js_context->evalFile(script.path.data());
			}
        }
    } catch (qjs::exception) {
        auto exc = js_context->getException();
        std::cerr << (std::string) exc << std::endl;
        if((bool) exc["stack"])
            std::cerr << (std::string) exc["stack"] << std::endl;
    }
}

void ScriptPlugin::OnDocumentUnload(ElementDocument* document) {
//	document_.reset();
    if (document->GetContext() != context_) return;
}

void ScriptPlugin::OnElementCreate(Element* element) {
//	element->GetParentNode()
}

void ScriptPlugin::OnElementDestroy(Element* element) {


}

ScriptPlugin::~ScriptPlugin() {
    js_context_.reset();
}

//void ScriptPlugin::FreshJsContext() {
//	js_context_.reset();
//    js_context_ = MakeUnique<qjs::Context>(*GetRunTime());
//    Glue(js_context_.get());
//}

}

}