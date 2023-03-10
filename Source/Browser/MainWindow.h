//
// Created by titto on 2022/12/29.
//

#ifndef RMLUI_MAINWINDOW_H
#define RMLUI_MAINWINDOW_H

#include "RmlUi/Core/Types.h"
#include "co/co/event.h"
#include "TabManager.h"
#include "BrowserWidget.h"

namespace Rml {

class URL;

namespace Browser {

class MainWindow;

static MainWindow* instance;

void OpenInCurrentTab(Context* context, const URL& url);
void OpenInNewTab(Context* context, const URL& url);

class MainWindow : public TabManager::Delegate, public BrowserWidget::Delegate {
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
    void ProcessEvent();

	void Close();
	void WaitForClose();

	TabManager* tab_manager() { return tab_manager_.get(); }

	/* TabManager Delegate */
	void OnTabRun(Tab* tab) override;
	void OnTabFresh(Tab* tab) override;
	void OnTabStopRunning(Tab* tab) override;
    void OnTabActive(Tab* tab) override;
    void OnTabUnActive(Tab* tab) override;


	/* BrowserWidget Delegate */
	void DoTabFocus(const String& tab_id) override;
	void DoTabRemove(const String& tab_id) override;
	void DoTabEnterUrl(const String& tab_id, const String& url) override;
	void DoTabOpenNew(const String& url) override;

private:

    UniquePtr<TabManager> tab_manager_;
	UniquePtr<BrowserWidget> browser_widget_;
	co::Event close_event_;
};

}
}



#endif // RMLUI_MAINWINDOW_H
