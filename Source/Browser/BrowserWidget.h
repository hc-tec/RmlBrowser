//
// Created by titto on 2023/1/7.
//

#ifndef RMLUI_BROWSERWIDGET_H
#define RMLUI_BROWSERWIDGET_H

#include <RmlUi/Core/Types.h>

#include "Collections.h"

namespace co {

class Scheduler;

}

namespace qjs {

class Context;

}

namespace Rml {

class Context;
class ElementDocument;

namespace Script {

class ScriptPlugin;

}

const String widget_rml_ = "/home/titto/CProjects/RmlUi5.0/Source/Browser/BrowserWidgetAssets/widget.rml";

static const char* BROWSER_WIDGET_ID = "browser-widget";

namespace Browser {

class BrowserWidget {
public:
	class Delegate {
	public:
		virtual void DoTabFocus(const String& tab_id) = 0;
        virtual void DoTabRemove(const String& tab_id) = 0;
        virtual void DoTabOpenNew(const String& url) = 0;
        virtual void DoTabEnterUrl(const String& tab_id, const String& url) = 0;
	};

    BrowserWidget(Delegate* delegate);
    ~BrowserWidget();

    int Initialize();
    void Render();
    void Run();

    qjs::Context* js_context();
private:
    Delegate* delegate_;

    co::Scheduler* scheduler;
    Rml::Context* context_;
    Rml::ElementDocument* document_;
    UniquePtr<Rml::Script::ScriptPlugin> script_plugin_;
    UniquePtr<Collections> collections_;
	bool running_;
};

}
}

#endif // RMLUI_BROWSERWIDGET_H
