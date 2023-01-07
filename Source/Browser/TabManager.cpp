//
// Created by titto on 2023/1/6.
//

#include "TabManager.h"

#include "Tab.h"

namespace Rml {

namespace Browser {

TabManager::TabManager()
		: tab_id_(1) {}

Tab* TabManager::NewTab(const URL& url) {
	String new_tab_id("Tab-");
	new_tab_id.append(std::to_string(tab_id_++));
	UniquePtr<Tab> new_tab_ptr = MakeUnique<Tab>(new_tab_id, url);
	new_tab_ptr->SetDelegate(this);
	Tab* new_tab = new_tab_ptr.get();
	tab_map_[new_tab_id] = std::move(new_tab_ptr);
	return new_tab;
}

Tab* TabManager::GetTabByContext(Context* context) {
    auto it = context_tab_map_.find(context);
    if (it == context_tab_map_.end()) return nullptr;
    return it->second;
}

void TabManager::OnInitialize(Tab* tab) {
	Context* context = tab->context();
	context_tab_map_[context] = tab;
}

void TabManager::OnRun(Tab* tab) {}

void TabManager::OnDestroy(Tab* tab) {
    Log::Message(Log::LT_DEBUG, "%s destroyed", tab->tab_id().data());
    Context* context = tab->context();
    context_tab_map_.erase(context);
}

void TabManager::OnFresh(Tab* tab) {}

void TabManager::OnStopRunning(Tab* tab) {
	Log::Message(Log::LT_DEBUG, "%s closed", tab->tab_id().data());
	String tab_id = tab->tab_id();
	auto it = tab_map_.find(tab_id);
	if (it == tab_map_.end()) return;
	it->second.reset();
}

void TabManager::CloseTab(const String& tab_id) {
    auto it = tab_map_.find(tab_id);
    if (it == tab_map_.end()) return;
    it->second->StopRunning();
}

void TabManager::CloseAllTabs() {
	for(auto& [_, tab] : tab_map_) {
		tab->StopRunning();
	}
}


}

}
