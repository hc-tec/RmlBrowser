//
// Created by titto on 2022/12/30.
//

#include "ScriptPlugin.h"
#include "Dom/XMLNodeHandlerAnchor.h"
#include "Dom/XMLNodeHandlerImg.h"
#include "CustomComponent/XMLNodeHandlerSlot.h"
#include "CustomComponent/XMLNodeHandlerCustomComponent.h"
#include "Glue.h"
#include "Net/Network.h"
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/Factory.h"
#include "RunTime.h"
#include "core/http/response/http_response_info.h"
#include <Core/ResourceLoader.h>
#include <iostream>
#include "RmlUi/Core/StyleSheetContainer.h"
namespace Rml {

namespace Script {

ScriptPlugin::ScriptPlugin()
		: js_runtime_(nullptr) {
//    static co::mutex js_mutex;
//    auto _ = co::mutex_guard(js_mutex);
    js_runtime_ = GetRunTime(); // new qjs::Runtime();
    js_context_ = MakeUnique<qjs::Context>(js_runtime_->rt);
    Glue(js_context_.get());
    js_document_element_instancer_ = MakeShared<JsDocumentElementInstancer>(this);
    Factory::RegisterElementInstancer("body", js_document_element_instancer_.get());
    XMLParser::RegisterNodeHandler("a", MakeShared<XMLNodeHandlerAnchor>());
    XMLParser::RegisterNodeHandler("img", MakeShared<XMLNodeHandlerImg>());
    XMLParser::RegisterNodeHandler("slot", MakeShared<XMLNodeHandlerSlot>());
    XMLParser::RegisterNodeHandler("script", js_document_element_instancer_);
//    XMLParser::RegisterNodeHandler("template", MakeShared<XMLNodeHandlerDefault>());

    String script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/rue.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/utils.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/custom_component.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/rml-theme.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/rml-alert.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/rml-button.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/rml-input.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/rml-select.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/rml-avatar.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/rml-notification.js";
    builtin_scripts_.push_back(script);
    script = "/home/titto/CProjects/RmlUi5.0/Source/Script/Builtin/Components/rml-dialog.js";
    builtin_scripts_.push_back(script);


    for (auto& path : builtin_scripts_) {
        LoadExternJs(js_context_.get(), path);
    }

	builtin_scripts_.clear();

	js_context_->global()["CCustomSlotSet"] = [](const String& id, const String& rml){
		XMLNodeHandlerCustomComponent::SetSlotData(id, rml);
	};
    js_context_->global()["CCustomNeedParse"] = [](){
      return XMLNodeHandlerCustomComponent::NeedParse();
    };
    js_context_->global()["CCustomGetParseEl"] = [](){
      return XMLNodeHandlerCustomComponent::GetParseEl();
    };
}

void ScriptPlugin::LoadJs() {
    qjs::Context* js_context = js_context_.get();
    DocumentHeader::ResourceList scripts = js_document_element_instancer_->GetScripts();
    try {
        js_context->global()["executing"] = true;
        for (auto& script : scripts) {
            if (script.is_inline) {
                js_context->eval(script.content, script.path.data());
            } else {
                LoadExternJs(js_context, script.path);
            }
        }
        js_context->global()["executing"] = false;
    } catch (qjs::exception) {
        auto exc = js_context->getException();
        std::cerr << (std::string) exc << std::endl;
        if((bool) exc["stack"])
            std::cerr << (std::string) exc["stack"] << std::endl;
    }
    js_document_element_instancer_->scripts_.clear();
}

void ScriptPlugin::OnDocumentLoad(ElementDocument* document) {
    qjs::Context* js_context = js_context_.get();
    js_context->global()["document"] = document;
	if (!register_components_.empty())
	{
		try
		{
			auto func = (std::function<void()>)js_context->eval("parseCustomComponent");
			func();
		} catch (qjs::exception)
		{
			auto exc = js_context->getException();
			std::cerr << (std::string)exc << std::endl;
			if ((bool)exc["stack"])
				std::cerr << (std::string)exc["stack"] << std::endl;
		}
	}

    LoadCustomComponentResource(document);
	LoadJs();
}

ScriptPlugin::~ScriptPlugin() {
    js_context_.reset();
}

void ScriptPlugin::LoadExternJs(qjs::Context* js_context, const std::string& path) {
    URL url(path);
	if (url.GetProtocol() == "file")
	{
		js_context->evalFile(path.data());
	} else {
		ResourceLoader* resource_loader = ResourceLoader::Get();
		NetStreamFile file;
		resource_loader->WaitForResource(path, &file);
        file.Open(path);
        char* buf = new char[file.GetSize()];
        file.Read(buf, file.GetSize());
        js_context->eval(buf, url.GetFileName().data());
		delete[] buf;
	}
}

void ScriptPlugin::OnHeadParseFinish() {
	LoadJs();
    UnorderedMap<String, CustomComponentAssert>& assets = XMLNodeHandlerCustomComponent::GetComponentAssets();
    for (auto& [key, value] : assets) {
        register_components_.push_back(key);
    }
}

void ScriptPlugin::LoadCustomComponentResource(ElementDocument* document) {
    SharedPtr<StyleSheetContainer> new_sheet = MakeShared<StyleSheetContainer>();
    const StyleSheetContainer* origin_sheet = document->GetStyleSheetContainer();
	new_sheet->MergeStyleSheetContainer(*origin_sheet);
	// css load once
    UnorderedMap<String, CustomComponentAssert>& assets = XMLNodeHandlerCustomComponent::GetComponentAssets();
    for (auto& key : register_components_) {
        CustomComponentAssert& value = assets[key];
        SharedPtr<StyleSheetContainer> style_sheet = Factory::InstanceStyleSheetString(value.rcss);
		if (style_sheet == nullptr) continue;
        new_sheet->MergeStyleSheetContainer(*(style_sheet.get()));
	}
    document->SetStyleSheetContainer(std::move(new_sheet));

    qjs::Context* js_context = js_context_.get();
	// but js execute times with regard to how many times the component use by the developer
    Vector<CustomComponentAssert>& use_assets = XMLNodeHandlerCustomComponent::GetUseComponentAssets();
	for (auto& value : use_assets) {
		try
		{
            js_context->eval(value.script, value.name.data());
        } catch (qjs::exception) {
            auto exc = js_context->getException();
            std::cerr << (std::string) exc << std::endl;
            if((bool) exc["stack"])
                std::cerr << (std::string) exc["stack"] << std::endl;
        }
	}
    XMLNodeHandlerCustomComponent::ClearUseComponentAssets();
}

}

}