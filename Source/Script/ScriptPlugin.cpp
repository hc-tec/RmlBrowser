//
// Created by titto on 2022/12/30.
//

#include "ScriptPlugin.h"

#include <iostream>

#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/Factory.h"
#include "RmlUi/Core/ElementDocument.h"

#include "Glue.h"
#include "Dom/JsDocumentElementInstancer.h"
#include "Dom/XMLNodeHandlerAnchor.h"

namespace Rml {

namespace Script {

int ScriptPlugin::GetEventClasses() { return Plugin::GetEventClasses(); }

void ScriptPlugin::OnInitialise() {
    js_document_element_instancer_ = MakeShared<JsDocumentElementInstancer>();
    Factory::RegisterElementInstancer("body", js_document_element_instancer_.get());
	XMLParser::RegisterNodeHandler("a", MakeShared<XMLNodeHandlerAnchor>());
    XMLParser::RegisterNodeHandler("script", js_document_element_instancer_);
	Glue();
}
void ScriptPlugin::OnShutdown() { Plugin::OnShutdown(); }

void ScriptPlugin::OnContextCreate(Context* context) {
    context_ = context;
	std::cout << "Register ScriptPlugin" << std::endl;
}

void ScriptPlugin::OnContextDestroy(Context* context) {
	if (context == context_) context_ = nullptr;
}

void ScriptPlugin::OnDocumentOpen(Context* context, const String& document_path) {

}

void ScriptPlugin::OnDocumentLoad(ElementDocument* document) {
    qjs::Context* js_context = GetContext();
    js_context->global()["document"] = document;
	DocumentHeader::ResourceList scripts = js_document_element_instancer_->GetScripts();
    try {
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
}

void ScriptPlugin::OnElementCreate(Element* element) {
//	element->GetParentNode()
}

void ScriptPlugin::OnElementDestroy(Element* element) {


}

ScriptPlugin::~ScriptPlugin() {
	ScriptPlugin* instance = GetInstance();
	delete instance;
}

}

}