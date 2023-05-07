//
// Created by titto on 2023/5/5.
//

#ifndef RMLUI_BROWSERPLUGINREGISTRY_H
#define RMLUI_BROWSERPLUGINREGISTRY_H

namespace Rml {

namespace Browser {

class Tab;
class BrowserPlugin;

class BrowserPluginRegistry {
public:
    static void RegisterPlugin(BrowserPlugin* plugin);
    static void UnregisterPlugin(BrowserPlugin* plugin);

	static void NotifyTabActive(Tab* tab);
    static void NotifyTabStopRunning(Tab* tab);
};

}
}
#endif // RMLUI_BROWSERPLUGINREGISTRY_H
