//
// Created by titto on 2022/12/29.
//

#ifndef RMLUI_MAINWINDOW_H
#define RMLUI_MAINWINDOW_H

#include "RmlUi/Core/Types.h"
#include "co/co/event.h"

namespace Rml {

class URL;

namespace Browser {

class TabManager;
class MainWindow;
class BrowserWidget;

static MainWindow* instance;

void OpenInCurrentTab(Context* context, const URL& url);
void OpenInNewTab(Context* context, const URL& url);

class MainWindow {
public:
	MainWindow();
	~MainWindow();

	static MainWindow* GetInstance() {
		if (instance == nullptr) {
            instance = new MainWindow();
		}
		return instance;
	}

	bool Initialize();

	void Close();
	void WaitForClose();

	TabManager* tab_manager() { return tab_manager_.get(); }
private:

    UniquePtr<TabManager> tab_manager_;
	UniquePtr<BrowserWidget> browser_widget_;
	co::Event close_event_;
};

}
}



#endif // RMLUI_MAINWINDOW_H
