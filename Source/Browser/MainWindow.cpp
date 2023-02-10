//
// Created by titto on 2022/12/29.
//

#include "MainWindow.h"
#include "../Script/RunTime.h"
#include "../Script/ScriptPlugin.h"
#include <RmlUi/Core.h>
#include <RmlUi/Config/Config.h>
#include <RmlUi_Backend.h>
#include <Shell.h>
#include <co/co.h>
#include "TabManager.h"
#include "BrowserWidget.h"
#include "Glue.h"

const int window_width = 1550;
const int window_height = 760;

namespace Rml {
namespace Browser {

MainWindow::MainWindow()
    : tab_manager_(MakeUnique<TabManager>(this)),
      browser_widget_(MakeUnique<BrowserWidget>(this)) {
	RegisterBrowserGlueFunc();
	Initialize();
    browser_widget_->Run();
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
	qjs::Context* js_context = browser_widget_->js_context();
	auto func = (std::function<void(qjs::Value)>) js_context->eval("TAB_MANAGER_ADD_TAB");
	auto obj = js_context->newObject();
	obj["id"] = tab->tab_id();
    obj["title"] = tab->title();
    obj["url"] = tab->document()->GetSourceURL();
	func(obj);
}

void MainWindow::OnTabFresh(Tab* tab) {
    qjs::Context* js_context = browser_widget_->js_context();
    auto func = (std::function<void(qjs::Value)>) js_context->eval("TAB_MANAGER_FRESH_TAB");
    auto obj = js_context->newObject();
    obj["id"] = tab->tab_id();
    obj["title"] = tab->title();
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
    Rml::Browser::Tab* tab = tab_manager_->NewTab("");
    tab->Run();
    DoTabFocus(tab->tab_id());
}

void OpenInCurrentTab(Context* context, const URL& url) {
    MainWindow* window = MainWindow::GetInstance();
	TabManager* tab_manager = window->tab_manager();
	Tab* tab = tab_manager->GetTabByContext(context);
	tab->SetUrl(url);
	tab->Fresh();
}

void OpenInNewTab(Context* context, const URL& url) {

}


}

namespace Script {

void AnchorOpenInCurrentTabCallback(Context* context, const URL& url) {
	Browser::OpenInCurrentTab(context, url);
}
void AnchorOpenInNewTabCallback(Context* context, const URL& url) {
    Browser::OpenInNewTab(context, url);
}

}
}

// int main
DEF_main(argc, argv) {
    Rml::Browser::MainWindow* window = Rml::Browser::MainWindow::GetInstance();
    Rml::Browser::TabManager* tab_manager = window->tab_manager();
//    Rml::Browser::Tab* tab2 = tab_manager->NewTab("/home/titto/CProjects/RmlUi5.0/Samples/basic/animation/data/animation.rml");
//    tab2->Run();
    Rml::Browser::Tab* tab1 = tab_manager->NewTab("/home/titto/CProjects/RmlUi5.0/Samples/web/chromium-intro/thread.rml");
    tab1->Run();
//    Rml::Browser::Tab* tab3 = tab_manager->NewTab("/home/titto/CProjects/RmlUi5.0/Samples/web/chromium-intro/index.rml");
//    tab3->Run();
    Rml::Browser::Tab* tab4 = tab_manager->NewTab("http://127.0.0.1:8000/thread.rml");
    tab4->Run(true);

	window->WaitForClose();
	delete window;
}


