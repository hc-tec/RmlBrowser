//
// Created by titto on 2023/1/5.
//

#ifndef RMLUI_TAB_H
#define RMLUI_TAB_H

#include <co/co.h>
#include "RmlUi/Config/Config.h"
#include "RmlUi/Core/URL.h"
#include "../Script/ScriptPlugin.h"

namespace Rml {


namespace Browser {

class Tab {
public:
	class Delegate {
	public:
		virtual void OnInitialize(Tab* tab) = 0;
        virtual void OnRun(Tab* tab) = 0;
        virtual void OnDestroy(Tab* tab) = 0;
        virtual void OnFresh(Tab* tab) = 0;
        virtual void OnStopRunning(Tab* tab) = 0;
	};
	Tab(const String& tab_id, const URL& url);
    int Initialize();
	void Render();
    void Run();
    void Fresh();
	void StopRunning();

	void Show();
    void Hide();

	void SetDelegate(Delegate* delegate) { delegate_ = delegate; }
	const String& tab_id() { return tab_id_; }

	void SetUrl(const URL& url) { url_ = url; }
	const URL& url() { return url_; }

	Rml::Context* context() { return context_; }
private:
    void Destroy();

    co::Scheduler* scheduler;
	Delegate* delegate_;
	String tab_id_;
	URL url_;

    Rml::Context* context_;
    Rml::ElementDocument* document_;
    Rml::Script::ScriptPlugin* script_plugin_;

	bool active_;
	bool rendering_;
	bool running_;
};

}
}


#endif // RMLUI_TAB_H