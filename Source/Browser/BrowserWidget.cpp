//
// Created by titto on 2023/1/7.
//

#include "BrowserWidget.h"


#include <RmlUi/Core.h>
#include <RmlUi_Backend.h>
#include <Shell.h>
#include <co/co.h>

#include "../Script/ScriptPlugin.h"


const int window_width = 1550;
const int window_height = 760;

namespace Rml {

namespace Browser {

BrowserWidget::BrowserWidget()
		: scheduler(co::schedulers().at(0)) {}

int BrowserWidget::Initialize() {
    script_plugin_ = MakeUnique<Script::ScriptPlugin>();
    Rml::RegisterPlugin(script_plugin_.get());

    // Create the main RmlUi context.
    context_ = Rml::CreateContext("browser-widget", Rml::Vector2i(window_width, window_height));
    if (!context_)
    {
        return -1;
    }
    Backend::RegisterContext(context_, scheduler);
    // Load the demo document.
    document_ = context_->LoadDocument(widget_rml_);
	if (document_)
	{
        document_->Show();
		document_->SetId("browser-widget");
	}
}

void BrowserWidget::Render() {
	RMLUI_ASSERT(context_)
// Handle input and window events.
//    bool is_backend_running = Backend::ProcessEvents(context_, &Shell::ProcessKeyDownShortcuts);
//    if (!is_backend_running) {
//        running_ = false;
//    }

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
    Rml::UnregisterPlugin(script_plugin_.get());
	script_plugin_.reset();
}

}

}
