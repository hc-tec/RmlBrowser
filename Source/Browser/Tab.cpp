//
// Created by titto on 2023/1/5.
//

#include "Tab.h"

#include "../Script/RunTime.h"
#include "../Script/ScriptPlugin.h"
#include "../Script/Dom/Ownership.h"
#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi/Config/Config.h>
#include <RmlUi_Backend.h>
#include <Shell.h>
#include <co/co.h>
#include "MainWindow.h"

const int window_width = 1550;
const int window_height = 760;

namespace Rml {
namespace Browser {

Tab::Tab(const String& tab_id, const URL& url)
    : delegate_(nullptr),
		tab_id_(tab_id),
		url_(url),
		context_(nullptr),
		script_plugin_(nullptr),
		rendering_(true),
		running_(true) {
}

int Tab::Initialize() {
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
	return 0;
}

void Tab::RunInternal() {
    Initialize();
    // Load and show the demo document.
    Rml::ElementDocument* document = context_->LoadDocument(url_.GetURL());
	document->SetId(tab_id_);
    if (document)
        document->Show();
    rendering_ = true;
    if (delegate_) delegate_->OnRun(this);
    while (rendering_ && running_)
    {
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
    }

	Destroy();
}

void Tab::Run() {
    do {
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
        RunInternal();
    } while (running_);
    if (delegate_) delegate_->OnStopRunning(this);
}

void Tab::Fresh() {
    rendering_ = false;
    Log::Message(Log::LT_DEBUG, "[Tab = %s] Reload", tab_id_.data());
    if (delegate_) delegate_->OnFresh(this);
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

void Tab::StopRunning() {
	rendering_ = false;
	running_ = false;
}

}
}