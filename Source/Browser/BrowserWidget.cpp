//
// Created by titto on 2023/1/7.
//

#include "BrowserWidget.h"

#include <iostream>
#include <utility>
#include <RmlUi_Backend.h>
#include "log/logging.h"
#include "../Script/ScriptPlugin.h"
#include "RmlContext.h"
#include "RenderScheduler.h"
namespace Rml {

namespace Browser {

BrowserWidget::BrowserWidget(Delegate* delegate)
		: delegate_(delegate),
		scheduler(RenderScheduler::Get()),
		context_(nullptr),
		document_(nullptr),
		collections_(MakeUnique<Collections>()),
		running_(true) {}

int BrowserWidget::Initialize() {
    // Create the main RmlUi context.
    context_ = RmlContext::Get();
    if (!context_)
    {
        return -1;
    }

    script_plugin_ = MakeUnique<Script::ScriptPlugin>();

    qjs::Context* js_context = script_plugin_->js_context();
    Glue();
    // Load the demo document.
    document_ = context_->LoadDocument(widget_rml_);
	if (document_)
	{
        script_plugin_->OnDocumentLoad(document_);
        document_->Show();
		document_->SetId(BROWSER_WIDGET_ID);
	}

    auto func = (std::function<void(qjs::Value)>) js_context->eval("STARS_LOADED");
    Vector<Star> stars;
    collections_->Get(stars);
    JSValue obj = qjs::js_traits<Vector<Star>>::wrap(js_context->ctx, stars);
    qjs::Value v {js_context->ctx, JS_DupValue(js_context->ctx, obj)};
    func(v);
}

void BrowserWidget::Run() {
    scheduler->go([&](){
        Initialize();
    });
}

BrowserWidget::~BrowserWidget() {
	running_ = false;
    script_plugin_.reset();
	collections_.reset();
}

qjs::Context* BrowserWidget::js_context() { return script_plugin_->js_context(); }

void BrowserWidget::Glue() {
    qjs::Context* js_context = script_plugin_->js_context();
    js_context->global()["CStarAdd"] = [&](String title, String icon, String url){
      Star s = {
          .title = std::move(title),
          .icon = std::move(icon),
          .url = std::move(url)
      };
      collections_->Collect(s);
    };
    js_context->global()["CFocusTab"] = [&](const Rml::String& tab_id){
      delegate_->DoTabFocus(tab_id);
      std::cout << tab_id << std::endl;
    };
    js_context->global()["CRemoveTab"] = [&](const Rml::String& tab_id, const Rml::String& focus_id){
      delegate_->DoTabRemove(tab_id);
      delegate_->DoTabFocus(focus_id);
      std::cout << tab_id << std::endl;
    };

    qjs::Value rml = js_context->newObject();
    rml["extension"] = js_context->newObject();
    rml["extension"]["dispatchClickEvent"] = [this](const String& name, qjs::Value event) {
		delegate_->DoExtensionClick(name, std::move(event));
	};
//    rml["dom"] = js_context->newObject();
//    rml["tab"] = js_context->newObject();
//    rml["net"] = js_context->newObject();
    js_context->global()["rml"] = rml;
}

}

}
