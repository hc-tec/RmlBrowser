//
// Created by titto on 2023/1/5.
//

#ifndef RMLUI_TAB_H
#define RMLUI_TAB_H

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
    void Run();

	void StopRunning();
	void Fresh();

	void SetDelegate(Delegate* delegate) { delegate_ = delegate; }
	const String& GetTabId() { return tab_id_; }
	void SetUrl(const URL& url) { url_ = url; }
	const URL& Url() { return url_; }
	Rml::Context* GetContext();
private:
    void RunInternal();
    void Destroy();

	Delegate* delegate_;
	String tab_id_;
	URL url_;
    Rml::Context* context_;
    Rml::Script::ScriptPlugin* script_plugin_;

	bool rendering_;
	bool running_;
};

}
}


#endif // RMLUI_TAB_H
