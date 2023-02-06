//
// Created by titto on 2023/1/7.
//

#include "BrowserWidget.h"

#include <iostream>
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
		running_(true) {}

int BrowserWidget::Initialize() {
    // Create the main RmlUi context.
    context_ = Rml::CreateContext(BROWSER_WIDGET_ID, Rml::Vector2i(window_width, window_height));
    if (!context_)
    {
        return -1;
    }
    Backend::RegisterContext(context_, scheduler);

    script_plugin_ = MakeUnique<Script::ScriptPlugin>(context_);
    Rml::RegisterPlugin(script_plugin_.get());

    qjs::Context* js_context = script_plugin_->js_context();
    js_context->global()["log"] = [](const Rml::String& str){
      std::cout << str << std::endl;
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
    js_context->global()["COpenTabWithUrl"] = [&](const Rml::String& tab_id, const Rml::String& url){
		if (tab_id == "-1") {
			delegate_->DoTabOpenNew(url);
		} else
		{
			delegate_->DoTabEnterUrl(tab_id, url);
		}
        std::cout << tab_id << std::endl;
    };
    // Load the demo document.
    document_ = context_->LoadDocument(widget_rml_);
	if (document_)
	{
        document_->Show();
		document_->SetId(BROWSER_WIDGET_ID);
	}
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
}

qjs::Context* BrowserWidget::js_context() { return script_plugin_->js_context(); }

}

}
