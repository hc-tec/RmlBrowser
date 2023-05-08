//
// Created by titto on 2023/5/5.
//

#ifndef RMLUI_EXTENSION_CONTROLLER_H
#define RMLUI_EXTENSION_CONTROLLER_H

#include "Net/Network.h"
#include "core/url_request/url_request_observer.h"
#include <RmlUi/Core/EventListener.h>
#include <Browser/BrowserPlugin.h>
#include <Script/ScriptPlugin.h>

namespace Rml {

namespace Browser {

class ExtensionManifest;

class ExtensionController :
	public BrowserPlugin,
	public net::URLRequestObserver,
    public EventListener {
public:
	ExtensionController(UniquePtr<ExtensionManifest> manifest);
	~ExtensionController() override;

	bool Filter(Element* element);
    bool Filter(Tab* tab);
    bool Filter(const String& tab_id);

	void set_interested_tab(Tab* tab);

	/* BrowserPlugin */
	void OnInitialise() override;
	void OnShutdown() override;
	void OnContextCreate(Context* context) override;
	void OnContextDestroy(Context* context) override;
	void OnDocumentOpen(Context* context, const String& document_path) override;
	void OnDocumentLoad(ElementDocument* document) override;
	void OnDocumentUnload(ElementDocument* document) override;
	void OnElementCreate(Element* element) override;
	void OnElementDestroy(Element* element) override;
	void OnTabStopRunning(Tab* tab) override;
	void OnTabActive(Tab* tab) override;

	/* URLRequestObserver */
	void OnRequestStart(net::URLRequest* request, net::HttpRequestInfo* requestInfo) override;
	void OnRequestEnd(net::URLRequest* request, net::HttpRequestInfo* requestInfo, net::HttpResponseInfo* responseInfo) override;
	void OnRequestError(net::URLRequest* request, int err_code) override;

	void OnExtensionIconClick(Tab* tab, const String& name, qjs::Value event);

    ExtensionManifest* manifest() { return manifest_.get(); }

	/* EventListener */
	void ProcessEvent(Event& event) override;

private:

    void Glue();

	Tab* cur_tab_;

    Rml::Context* context_;
    Rml::ElementDocument* document_;
    UniquePtr<Script::ScriptPlugin> script_plugin_;
	UniquePtr<ExtensionManifest> manifest_;

	bool close_cause_unactive_;
	bool is_active_;
	int unactive_time_;
};

}
}
#endif // RMLUI_EXTENSION_CONTROLLER_H
