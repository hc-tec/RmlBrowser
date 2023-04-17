//
// Created by titto on 2023/1/6.
//

#include "TabManager.h"

#include "Tab.h"
#include "RmlUi/Core/Log.h"

namespace Rml {

namespace Browser {

TabManager::TabManager(Delegate* delegate)
		: delegate_(delegate),
		tab_id_(1) {}

Tab* TabManager::NewTab(const URL& url) {
	URL new_url = url;
	if (url.GetURL().empty()) {
		new_url.SetURL("/home/titto/CProjects/RmlUi5.0/Source/Browser/BlankTabAssets/index.rml");
	}
	String new_tab_id("Tab-");
	new_tab_id.append(std::to_string(tab_id_++));
	UniquePtr<Tab> new_tab_ptr = MakeUnique<Tab>(new_tab_id, new_url);
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
    if (delegate_) delegate_->OnTabRun(tab);
}

void TabManager::OnDestroy(Tab* tab) {
    Log::Message(Log::LT_DEBUG, "%s destroyed", tab->tab_id().data());
    Context* context = tab->context();
    context_tab_map_.erase(context);
}

void TabManager::OnFresh(Tab* tab) {
    if (delegate_) delegate_->OnTabFresh(tab);
}

void TabManager::OnStopRunning(Tab* tab) {
	Log::Message(Log::LT_DEBUG, "%s closed", tab->tab_id().data());
	String tab_id = tab->tab_id();
	auto it = tab_map_.find(tab_id);
	if (it == tab_map_.end()) return;
	tab_map_.erase(it);
    if (delegate_) delegate_->OnTabStopRunning(tab);
}

void TabManager::CloseTab(const String& tab_id) {
    auto it = tab_map_.find(tab_id);
    if (it == tab_map_.end()) return;
	if (tab_id == active_tab_->tab_id()) active_tab_ = nullptr;
    it->second->StopRunning();
}

void TabManager::CloseAllTabs() {
	for(auto& [_, tab] : tab_map_) {
		tab->StopRunning();
	}
}

void TabManager::OnActive(Tab* tab) {
    active_tab_ = tab;
    if (delegate_) delegate_->OnTabActive(tab);
}

void TabManager::OnUnActive(Tab* tab) {
    if (delegate_) delegate_->OnTabUnActive(tab);
}

void TabManager::FocusTab(const String& tab_id) {
	auto it = tab_map_.find(tab_id);
	if (it == tab_map_.end()) return;
	if (active_tab_)
	{
		if (tab_id == active_tab_->tab_id())
			return;
		active_tab_->Hide();
	}
	it->second->Show();
}

void TabManager::RemoveTab(const String& tab_id) {
	CloseTab(tab_id);
}

Tab* TabManager::GetTabById(const String& tab_id) {
    auto it = tab_map_.find(tab_id);
    if (it == tab_map_.end()) return nullptr;
	return it->second.get();
}

void TabManager::OnDocumentLoad(Tab* tab, ElementDocument* document) {
	delegate_->OnDocumentLoad(tab, document);
}

}

}
