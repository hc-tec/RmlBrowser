//
// Created by titto on 2023/1/5.
//

#include "Tab.h"

#include "../Script/RunTime.h"
#include "../Script/Dom/Ownership.h"
#include "MainWindow.h"

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi/Config/Config.h>
#include <RmlUi_Backend.h>
#include <Shell.h>
#include <co/co.h>


const int window_width = 1550;
const int window_height = 760;

namespace Rml {
namespace Browser {

Tab::Tab(const String& tab_id, const URL& url)
    : scheduler(co::next_scheduler()),
		delegate_(nullptr),
		tab_id_(tab_id),
		url_(url),
		document_(nullptr),
		context_(nullptr),
		script_plugin_(nullptr),
        active_(false),
		rendering_(true),
		running_(true) {
}

int Tab::Initialize() {
    qjs::Context* js_context = Rml::Script::GetContext();
    js_context->global()["log"] = [](const Rml::String& str){
      std::cout << str << std::endl;
    };
    js_context->global()["reload"] = [&](){
      this->Fresh();
    };
    js_context->global()["window_exit"] = [&](){
      Rml::Browser::MainWindow* window = Rml::Browser::MainWindow::GetInstance();
      window->Close();
    };

    script_plugin_ = Rml::Script::GetInstance();
    Rml::RegisterPlugin(script_plugin_);

    // Create the main RmlUi context.
    context_ = Rml::CreateContext(tab_id_, Rml::Vector2i(window_width, window_height));
    if (!context_)
    {
        Destroy();
        return -1;
    }

    // The RmlUi debugger is optional but very useful. Try it by pressing 'F8' after starting this sample.
    Rml::Debugger::Initialise(context_);
    if (delegate_) delegate_->OnInitialize(this);
    // Load the demo document.
    document_ = context_->LoadDocument(url_.GetURL());
    document_->SetId(tab_id_);
	return 0;
}

void Tab::Render() {
    // Handle input and window events.
    bool is_backend_running = Backend::ProcessEvents(context_, &Shell::ProcessKeyDownShortcuts);
    if (!is_backend_running) {
        rendering_ = false;
        running_ = false;
    }

    // This is a good place to update your game or application.
    // Always update the context before rendering.
    context_->Update();

    // Prepare the backend for taking rendering commands from RmlUi and then render the context.
    Backend::BeginFrame();
    context_->Render();
    Backend::PresentFrame();
	co::sleep(50);
	if (rendering_)
        scheduler->go([&](){
          Render();
		});
}

void Tab::Destroy() {
    Rml::Script::ClearAllOwner();
    Rml::Script::GetContext(true);
    Rml::RemoveContext(tab_id_);
    Rml::Debugger::Shutdown();
    Rml::UnregisterPlugin(script_plugin_);
    Factory::ClearStyleSheetCache();
    if (delegate_) delegate_->OnDestroy(this);
}

void Tab::Run() {
	scheduler->go([&](){
        Initialize();
	});
}

void Tab::Fresh() {
    rendering_ = false;
    scheduler->go([&](){
        if (delegate_) delegate_->OnFresh(this);
        Destroy();
        Initialize();
		if (active_) Show();
    });
    Log::Message(Log::LT_DEBUG, "[Tab = %s] Reload", tab_id_.data());
}



void Tab::StopRunning() {
	rendering_ = false;
	running_ = false;
    scheduler->go([&](){
		Destroy();
		if (delegate_) delegate_->OnStopRunning(this);
	});
}

void Tab::Show() {
	active_ = true;
	rendering_ = true;
	scheduler->go([&](){
        RMLUI_ASSERT(document_)
		document_->Show();
		Render();
	});
}

void Tab::Hide() {
	active_ = false;
	rendering_ = false;
	scheduler->go([&](){
		RMLUI_ASSERT(document_)
		document_->Hide();
	});
}

}
}