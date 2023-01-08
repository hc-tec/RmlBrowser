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

const int window_width = 1550;
const int window_height = 760;

namespace Rml {
namespace Browser {

MainWindow::MainWindow()
    : tab_manager_(MakeUnique<TabManager>()) {
	Initialize();
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
	return true;
}

MainWindow::~MainWindow() {
    tab_manager_.reset();
    Rml::Shutdown();
    Backend::Shutdown();
    Shell::Shutdown();
}

void MainWindow::Close() {
	tab_manager_->CloseAllTabs();
    close_event_.signal();
}

void MainWindow::WaitForClose() {
	close_event_.wait();
    co::sleep(500);
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


DEF_main(argc, argv) {
    Rml::Browser::MainWindow* window = Rml::Browser::MainWindow::GetInstance();
    Rml::Browser::TabManager* tab_manager = window->tab_manager();
	Rml::Browser::Tab* tab = tab_manager->NewTab("/home/titto/CProjects/RmlUi5.0/Samples/web/chromium-intro/index.rml");
    tab->Run();
	tab->Show();
	co::sleep(1500);
	tab->Hide();
    co::sleep(1500);
	tab->Show();
	co::sleep(1500);
    tab->Hide();
    co::sleep(5000);
    tab->Show();
	window->WaitForClose();
	delete window;
}