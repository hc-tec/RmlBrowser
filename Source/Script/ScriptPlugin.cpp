//
// Created by titto on 2022/12/30.
//

#include "ScriptPlugin.h"

#include <iostream>

#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/ElementDocument.h"

#include "Glue.h"

namespace Rml {

namespace Script {

int ScriptPlugin::GetEventClasses() { return Plugin::GetEventClasses(); }

void ScriptPlugin::OnInitialise() {
	Glue();
}
void ScriptPlugin::OnShutdown() { Plugin::OnShutdown(); }

void ScriptPlugin::OnContextCreate(Context* context) {
    context_ = context;
	std::cout << "Register ScriptPlugin: " << context_ << std::endl;
}

void ScriptPlugin::OnContextDestroy(Context* context) {
	if (context == context_) context_ = nullptr;
}

void ScriptPlugin::OnDocumentOpen(Context* context, const String& document_path) {

}

void ScriptPlugin::OnDocumentLoad(ElementDocument* document) {
    std::cout << "OnDocumentLoad" << std::endl;
    qjs::Context* js_context = GetContext();
    js_context->global()["document"] = document;
}

void ScriptPlugin::OnDocumentUnload(ElementDocument* document) {
//	document_.reset();
}

void ScriptPlugin::OnElementCreate(Element* element) {
//	element->GetParentNode()
    std::cout << "OnElementCreate: " << element->GetTagName() << "-" << element->GetId() << std::endl;
}

void ScriptPlugin::OnElementDestroy(Element* element) {


}

ScriptPlugin::~ScriptPlugin() {
	ScriptPlugin* instance = GetInstance();
	delete instance;
}

}

}