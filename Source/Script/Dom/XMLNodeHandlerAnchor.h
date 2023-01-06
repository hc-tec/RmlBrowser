//
// Created by titto on 2023/1/5.
//

#ifndef RMLUI_XMLNODEHANDLERANCHOR_H
#define RMLUI_XMLNODEHANDLERANCHOR_H

#include <utility>

#include "RmlUi/Core/XMLNodeHandler.h"
#include "RmlUi/Core/EventListener.h"
#include "RmlUi/Core/URL.h"

namespace Rml {

namespace Script {

//typedef void (*AnchorOpenInCurrentTabCallback)(Context* context, const URL& url);
//typedef void (*AnchorOpenInNewTabCallback)(Context* context, const URL& url);

using AnchorOpenInCurrentTabCallback = std::function<void(Context*, const URL&)>;
using AnchorOpenInNewTabCallback = std::function<void(Context*, const URL&)>;

static AnchorOpenInCurrentTabCallback AnchorOpenInCurrentTab;
static AnchorOpenInNewTabCallback AnchorOpenInNewTab;

static void SetAnchorOpenInCurrentTabCallback(AnchorOpenInCurrentTabCallback cb) {
    AnchorOpenInCurrentTab = std::move(cb);
}

static void SetAnchorOpenInNewTabCallback(AnchorOpenInNewTabCallback cb) {
    AnchorOpenInNewTab = std::move(cb);
}

class XMLNodeHandlerAnchor : public Rml::XMLNodeHandler, public Rml::EventListener {
public:
    Element* ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) override;
	void ProcessEvent(Event& event) override;
	bool ElementEnd(XMLParser* parser, const String& name) override;
	bool ElementData(XMLParser* parser, const String& data, XMLDataType type) override;

private:
	void OpenInCurrentTab(Context* context, const String& href);
    void OpenInNewTab(Context* context, const String& href);
};
}
}


#endif // RMLUI_XMLNODEHANDLERANCHOR_H
