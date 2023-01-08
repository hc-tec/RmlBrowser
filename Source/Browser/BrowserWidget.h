//
// Created by titto on 2023/1/7.
//

#ifndef RMLUI_BROWSERWIDGET_H
#define RMLUI_BROWSERWIDGET_H

#include <RmlUi/Core/Types.h>

namespace co {
class Scheduler;
}

namespace Rml {

class Context;
class ElementDocument;

namespace Script {

class ScriptPlugin;

}

const String widget_rml_ = "/home/titto/CProjects/RmlUi5.0/Source/Browser/BrowserWidgetAssets/widget.rml";

namespace Browser {

class BrowserWidget {
public:
    BrowserWidget();
    ~BrowserWidget();

    int Initialize();
    void Render();
    void Run();


private:

    co::Scheduler* scheduler;
    Rml::Context* context_;
    Rml::ElementDocument* document_;
    UniquePtr<Rml::Script::ScriptPlugin> script_plugin_;

	bool running_ = true;
};

}
}

#endif // RMLUI_BROWSERWIDGET_H
