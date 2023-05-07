//
// Created by titto on 2023/5/5.
//

#ifndef RMLUI_BROWSERPLUGIN_H
#define RMLUI_BROWSERPLUGIN_H

#include "Tab.h"
#include <RmlUi/Core/Plugin.h>

namespace Rml {

namespace Browser {

class BrowserPlugin : public Rml::Plugin {
public:
    virtual ~BrowserPlugin() {};
    virtual void OnTabStopRunning(Tab* tab);
    virtual void OnTabActive(Tab* tab);
};

}
}
#endif // RMLUI_BROWSERPLUGIN_H
