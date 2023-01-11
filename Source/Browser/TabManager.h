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
	class Delegate {
	public:
        virtual void OnTabRun(Tab* tab) = 0;
        virtual void OnTabFresh(Tab* tab) = 0;
        virtual void OnTabStopRunning(Tab* tab) = 0;
		virtual void OnTabActive(Tab* tab) = 0;
        virtual void OnTabUnActive(Tab* tab) = 0;
	};
    TabManager(Delegate* delegate);

	Tab* NewTab(const URL& url);

	Tab* GetTabByContext(Context* context);

	Tab* GetTabById(const String& tab_id);

	void FocusTab(const String& tab_id);
    void RemoveTab(const String& tab_id);

    void CloseTab(const String& tab_id);
	void CloseAllTabs();

	/* Tab Delegate */
	void OnInitialize(Tab* tab) override;
	void OnDestroy(Tab* tab) override;
	void OnFresh(Tab* tab) override;
	void OnStopRunning(Tab* tab) override;
	void OnActive(Tab* tab) override;
	void OnUnActive(Tab* tab) override;

private:
    Delegate* delegate_;
	std::atomic<int> tab_id_;

	UnorderedMap<String, UniquePtr<Tab>> tab_map_;
	UnorderedMap<Context*, Tab*> context_tab_map_;
	Tab* active_tab_;
};

}

}

#endif // RMLUI_TABMANAGER_H
