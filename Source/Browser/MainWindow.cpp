//
// Created by titto on 2022/12/29.
//

#include "MainWindow.h"
#include "../Script/RunTime.h"
#include "../Script/ScriptPlugin.h"
#include "BrowserWidget.h"
#include "Glue.h"
#include "History.h"
#include "RmlContext.h"
#include "RmlUi/Config/Config.h"
#include "RmlUi/Core.h"
#include "RmlUi/Core/Utils.h"
#include "RmlUi_Backend.h"
#include "Shell.h"
#include "TabManager.h"
#include "log/logging.h"
#include <Core/ResourceLoader.h>
#include <co/co.h>

namespace Rml {
namespace Browser {

MainWindow::MainWindow()
    : tab_manager_(MakeUnique<TabManager>(this)),
      browser_widget_(MakeUnique<BrowserWidget>(this)),
      extension_manager_(MakeUnique<ExtensionManager>(this)) {
	RegisterBrowserGlueFunc();
    RegisterHistoryGlueFunc();
	Initialize();

	RenderScheduler::Get()->go([this](){
		browser_widget_->Initialize();
		extension_manager_->LoadSingletonExtensions();
	});

}

bool MainWindow::Initialize() {

    // Initializes the shell which provides common functionality used by the included samples.
    if (!Shell::Initialize())
        return false;

    // Constructs the system and render interfaces, creates a window, and attaches the renderer.
    if (!Backend::Initialize("Rml Browser", window_width, window_height, true))
    {
        Shell::Shutdown();
        return false;
    }

    // Install the custom interfaces constructed by the backend before initializing RmlUi.
    Rml::SetSystemInterface(Backend::GetSystemInterface());
    Rml::SetRenderInterface(Backend::GetRenderInterface());

    // RmlUi initialisation.
    Rml::Initialise();

    // Fonts should be loaded before any documents are loaded.
    Shell::LoadFonts();
    ProcessEvent();
	return true;
}

MainWindow::~MainWindow() {
    tab_manager_.reset();
	browser_widget_.reset();
	extension_manager_.reset();
    Rml::Shutdown();
    Backend::Shutdown();
    Shell::Shutdown();
    Script::ClearAllOwners();
}

void MainWindow::Close() {
    Backend::RequestExit();
	tab_manager_->CloseAllTabs();
    close_event_.signal();
}

void MainWindow::WaitForClose() {
	RmlContext::Get();
	RmlContext::Render();
	close_event_.wait();
    co::sleep(500);
}

void MainWindow::ProcessEvent() {
	co::Scheduler* scheduler = co::schedulers()[0];
    scheduler->go([&](){
		bool running = Backend::ProcessEvents();
		if (running) {
            ProcessEvent();
		} else {
			Close();
		}
	});
}

void MainWindow::OnTabRun(Tab* tab) {
    ResourceLoader::Get()->Clear();
    His h = {
        .title = tab->title(),
        .icon = tab->document()->GetIcon(),
        .url = tab->url().GetURL()
    };
    URL url(h.icon);
    if (url.GetProtocol().substr(0, 4) == "file")
    {
        h.icon = "/" + Absolutepath(h.icon, "/home/titto/CProjects/RmlUi5.0/Source/Browser/BrowserWidgetAssets/");
    }

    History history_;
    history_.Collect(h);

	qjs::Context* js_context = browser_widget_->js_context();
	auto func = (std::function<void(qjs::Value)>) js_context->eval("TAB_MANAGER_ADD_TAB");
	auto obj = js_context->newObject();
	obj["id"] = tab->tab_id();
    obj["title"] = tab->title();
    obj["icon"] = tab->document()->GetIcon();
    obj["url"] = tab->document()->GetSourceURL();
	func(obj);
}

void MainWindow::OnTabFresh(Tab* tab) {
    qjs::Context* js_context = browser_widget_->js_context();
    auto func = (std::function<void(qjs::Value)>) js_context->eval("TAB_MANAGER_FRESH_TAB");
    auto obj = js_context->newObject();
    obj["id"] = tab->tab_id();
    obj["title"] = tab->title();
    obj["icon"] = tab->document()->GetIcon();
    obj["url"] = tab->url().GetURL();
    func(obj);
}

void MainWindow::OnTabStopRunning(Tab* tab) {
    qjs::Context* js_context = browser_widget_->js_context();
    auto func = (std::function<void(const String&)>) js_context->eval("TAB_MANAGER_REMOVE_TAB");
    func(tab->tab_id());
}

void MainWindow::OnTabActive(Tab* tab) {
    qjs::Context* js_context = browser_widget_->js_context();
    auto func = (std::function<void(const String&)>) js_context->eval("TAB_MANAGER_ON_TAB_ACTIVE");
	try
	{
		func(tab->tab_id());
	}catch (qjs::exception) {
        auto exc = js_context->getException();
        Log::Message(Log::LT_DEBUG, "%s", ((std::string) exc).data());
        if((bool) exc["stack"])
            Log::Message(Log::LT_DEBUG, "%s", ((std::string) exc["stack"]).data());
    }
}

void MainWindow::OnTabUnActive(Tab* tab) {}

void MainWindow::OnDocumentLoad(Tab* tab, ElementDocument* document) {
    qjs::Context* js_context = browser_widget_->js_context();
    auto func = (std::function<void(qjs::Value)>) js_context->eval("TAB_MANAGER_UPDATE_TAB_PARAMS");
    auto obj = js_context->newObject();
    obj["id"] = tab->tab_id();
    obj["title"] = document->GetTitle();
    obj["url"] = tab->url().GetURL();
//    func(obj);
}

void MainWindow::DoTabFocus(const String& tab_id) {
    tab_manager_->FocusTab(tab_id);
}

void MainWindow::DoTabRemove(const String& tab_id) {
    tab_manager_->RemoveTab(tab_id);
}

void MainWindow::DoTabEnterUrl(const String& tab_id, const String& url) {
	Tab* tab = tab_manager_->GetTabById(tab_id);
    tab->SetUrl(url);
    tab->Fresh();
}

void MainWindow::DoTabOpenNew(const String& url) {
    Rml::Browser::Tab* tab = tab_manager_->NewTab(url);
    extension_manager_->LoadEveryTabExtensions(tab);
    tab->Run(true);
    DoTabFocus(tab->tab_id());
}

void MainWindow::OnExtensionLoad(const Vector<Json::Value>& extension_info) {
	if (browser_widget_ == nullptr) return;
    qjs::Context* js_context = browser_widget_->js_context();
    auto func = js_context->global()["rml"]["extension"]["onLoad"];
	if (JS_IsUndefined(func.this_obj)) return;
	static_cast<std::function<void(const Vector<Json::Value>&)>>(func)(extension_info);
}

void MainWindow::DoExtensionClick(const String& name, qjs::Value event) {
    extension_manager_->DoExtensionClick(tab_manager_->active_tab(), name, std::move(event));
}

void OpenInCurrentTab(ElementDocument* document, const URL& url) {
    MainWindow* window = MainWindow::GetInstance();
	TabManager* tab_manager = window->tab_manager();
	Tab* tab = tab_manager->GetTabById(document->GetId());
	tab->SetUrl(url);
	tab->Fresh();
}

void OpenInNewTab(ElementDocument* document, const URL& url) {

}


}

namespace Script {

void AnchorOpenInCurrentTabCallback(ElementDocument* document, const URL& url) {
	Browser::OpenInCurrentTab(document, url);
}
void AnchorOpenInNewTabCallback(ElementDocument* document, const URL& url) {
    Browser::OpenInNewTab(document, url);
}

}
}

// int main
DEF_main(argc, argv) {
    Rml::Browser::MainWindow* window = Rml::Browser::MainWindow::GetInstance();
    Rml::Browser::TabManager* tab_manager = window->tab_manager();
//    Rml::Browser::Tab* tab2 = tab_manager->NewTab("/home/titto/CProjects/RmlUi5.0/Source/Browser/History/index.rml");
//    tab2->Run();
    Rml::RenderScheduler::Get()->go([window](){
      window->DoTabOpenNew("/home/titto/CProjects/RmlUi5.0/Samples/web/chromium-intro/ipc_1.rml");
	});

//    Rml::Browser::Tab* tab3 = tab_manager->NewTab("http://127.0.0.1:8000/ipc_1.rml");
//    tab3->Run(true);



	window->WaitForClose();
	delete window;
}


