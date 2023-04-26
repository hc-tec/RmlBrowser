//
// Created by titto on 2023/1/7.
//

#include "BrowserWidget.h"

#include <iostream>
#include <utility>
#include <RmlUi/Core.h>
#include <RmlUi_Backend.h>
#include <co/co.h>

#include "../Script/ScriptPlugin.h"


const int window_width = 1550;
const int window_height = 760;

namespace Rml {

namespace Browser {

BrowserWidget::BrowserWidget(Delegate* delegate)
		: delegate_(delegate),
		scheduler(co::schedulers()[0]),
		context_(nullptr),
		document_(nullptr),
		collections_(MakeUnique<Collections>()),
		running_(true) {}

int BrowserWidget::Initialize() {
    // Create the main RmlUi context.
    context_ = Rml::CreateContext(BROWSER_WIDGET_ID, Rml::Vector2i(window_width, window_height));
    if (!context_)
    {
        return -1;
    }
    Backend::RegisterContext(context_, scheduler);

    script_plugin_ = MakeUnique<Script::ScriptPlugin>(context_, nullptr);
    Rml::RegisterPlugin(script_plugin_.get());

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


    // Load the demo document.
    document_ = context_->LoadDocument(widget_rml_);
	if (document_)
	{
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

void BrowserWidget::Render() {
    if (!running_) return;
	RMLUI_ASSERT(context_)
    // This is a good place to update your game or application.
    // Always update the context before rendering.
    context_->Update();

    // Prepare the backend for taking rendering commands from RmlUi and then render the context.
    Backend::BeginFrame();
    context_->Render();
    Backend::PresentFrame();
//    co::sleep(40);
    scheduler->go([&](){
      Render();
    });
}

void BrowserWidget::Run() {
    scheduler->go([&](){
        Initialize();
        Render();
    });
}

BrowserWidget::~BrowserWidget() {
	running_ = false;
    Rml::RemoveContext(BROWSER_WIDGET_ID);
    Rml::UnregisterPlugin(script_plugin_.get());
    script_plugin_.reset();
	collections_.reset();
}

qjs::Context* BrowserWidget::js_context() { return script_plugin_->js_context(); }

}

}
