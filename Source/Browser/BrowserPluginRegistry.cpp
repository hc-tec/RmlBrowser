//
// Created by titto on 2023/5/5.
//

#include "BrowserPluginRegistry.h"

#include <Core/PluginRegistry.h>

#include "BrowserPlugin.h"
#include "Tab.h"

namespace Rml {

namespace Browser {

typedef Vector< BrowserPlugin* > PluginList;
static PluginList tab_plugins;

void BrowserPluginRegistry::RegisterPlugin(BrowserPlugin* plugin) {
    PluginRegistry::RegisterPlugin(plugin);
    tab_plugins.push_back(plugin);
}

void BrowserPluginRegistry::UnregisterPlugin(BrowserPlugin* plugin) {
    PluginRegistry::UnregisterPlugin(plugin);
    tab_plugins.erase(std::remove(tab_plugins.begin(), tab_plugins.end(), plugin), tab_plugins.end());
}

void BrowserPluginRegistry::NotifyTabActive(Tab* tab) {
	for (auto plugin : tab_plugins) {
		plugin->OnTabActive(tab);
	}
}

void BrowserPluginRegistry::NotifyTabStopRunning(Tab* tab) {
    for (auto plugin : tab_plugins) {
        plugin->OnTabStopRunning(tab);
    }
}

}

}