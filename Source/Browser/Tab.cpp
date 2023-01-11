//
// Created by titto on 2023/1/5.
//

#include "Tab.h"

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>
#include <RmlUi/Config/Config.h>
#include <RmlUi_Backend.h>
#include <Shell.h>
#include <co/co.h>

#include "../Script/RunTime.h"
#include "../Script/ScriptPlugin.h"
#include "MainWindow.h"


const int window_width = 1550;
const int window_height = 760;
const int browser_widget_height = 100;

namespace Rml {
namespace Browser {

Tab::Tab(const String& tab_id, const URL& url)
    : scheduler(co::schedulers().at(0)),
		delegate_(nullptr),
		tab_id_(tab_id),
		url_(url),
		document_(nullptr),
		context_(nullptr),
        active_(false),
		rendering_(true),
		running_(true) {
}

int Tab::Initialize() {
    Rml::Script::RegisterOwnershipObserver(this);
    // Create the main RmlUi context.
    context_ = Rml::CreateContext(tab_id_, Rml::Vector2i(window_width, window_height));
    if (!context_)
    {
        Destroy();
        return -1;
    }
    Backend::RegisterContext(context_, scheduler);

    script_plugin_ = MakeUnique<Script::ScriptPlugin>(context_);
    Rml::RegisterPlugin(script_plugin_.get());

    qjs::Context* js_context = script_plugin_->js_context();
    js_context->global()["log"] = [](const Rml::String& str){
      LOG << str;
    };
    js_context->global()["reload"] = [&](){
      this->Fresh();
    };
    js_context->global()["window_exit"] = [&](){
      Rml::Browser::MainWindow* window = Rml::Browser::MainWindow::GetInstance();
      window->Close();
    };

    // The RmlUi debugger is optional but very useful. Try it by pressing 'F8' after starting this sample.
//    Rml::Debugger::Initialise(context_);
    // Load the demo document.
    document_ = context_->LoadDocument(url_.GetURL());
    document_->SetId(tab_id_);
    using Rml::PropertyId;
    document_->SetProperty(PropertyId::Top, Rml::Property(browser_widget_height, Rml::Property::PX));
    if (delegate_) delegate_->OnInitialize(this);
	return 0;
}

void Tab::Render() {
	if (!rendering_) return;

    // This is a good place to update your game or application.
    // Always update the context before rendering.
    context_->Update();

    // Prepare the backend for taking rendering commands from RmlUi and then render the context.
    Backend::BeginFrame();
    context_->Render();
    Backend::PresentFrame();
//	co::sleep(50);
	if (rendering_)
        scheduler->go([&](){
          Render();
		});
}

void Tab::Destroy() {
    Backend::UnRegisterContext(context_);
    Rml::Script::UnRegisterOwnershipObserver(this);
    Rml::Script::ClearOwners(js_owner_list_);
	js_owner_list_.clear();
    Rml::RemoveContext(tab_id_);
    Rml::UnregisterPlugin(script_plugin_.get());
	script_plugin_.reset();
//    Rml::Debugger::Shutdown();
    Factory::ClearStyleSheetCache();
    if (delegate_) delegate_->OnDestroy(this);
}

void Tab::Run(bool show) {
	scheduler->go([&, show](){
        Initialize();
        if (show) Show();
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
		if (delegate_) delegate_->OnActive(this);
		Render();
	});
}

void Tab::Hide() {
	Backend::ClearFrame();
	active_ = false;
	rendering_ = false;
	scheduler->go([&](){
		RMLUI_ASSERT(document_)
		document_->Hide();
		if (delegate_) delegate_->OnUnActive(this);
	});
}

Tab::~Tab() {
	if (running_) Destroy();
	running_ = false;
}

qjs::Context* Tab::js_context() { return script_plugin_->js_context(); }

const String& Tab::title() { return document_->GetTitle(); }

void Tab::OnOwnerShift(std::any ptr) {
	if (script_plugin_ == nullptr) return;
    qjs::Context* js_ctx = js_context();
	auto executing = js_ctx->global()["executing"];
	if (executing.as<bool>())
	{
		js_owner_list_.push_back(ptr);
	}
}

}
}