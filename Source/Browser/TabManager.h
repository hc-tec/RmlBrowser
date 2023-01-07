//
// Created by titto on 2023/1/6.
//

#ifndef RMLUI_TABMANAGER_H
#define RMLUI_TABMANAGER_H

#include "RmlUi/Core/Types.h"
#include "Tab.h"
#include <atomic>

namespace Rml {

class Context;

namespace Browser {

class TabManager : public Tab::Delegate {
public:
    TabManager();

	Tab* NewTab(const URL& url);

	Tab* GetTabByContext(Context* context);

    void CloseTab(const String& tab_id);
	void CloseAllTabs();

	/* Tab Delegate */
	void OnInitialize(Tab* tab) override;
	void OnRun(Tab* tab) override;
	void OnDestroy(Tab* tab) override;
	void OnFresh(Tab* tab) override;
	void OnStopRunning(Tab* tab) override;

private:

	std::atomic<int> tab_id_;

	UnorderedMap<String, UniquePtr<Tab>> tab_map_;
	UnorderedMap<Context*, Tab*> context_tab_map_;
};

}

}

#endif // RMLUI_TABMANAGER_H
