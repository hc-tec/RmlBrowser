//
// Created by titto on 2023/1/5.
//

#ifndef RMLUI_XMLNODEHANDLERANCHOR_H
#define RMLUI_XMLNODEHANDLERANCHOR_H

#include "RmlUi/Core/XMLNodeHandler.h"
#include "RmlUi/Core/EventListener.h"

namespace Rml {

namespace Script {
class XMLNodeHandlerAnchor : public Rml::XMLNodeHandler, public Rml::EventListener {
public:
    Element* ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) override;
	void ProcessEvent(Event& event) override;

private:
	void OpenInCurrentTab(const String& href);
    void OpenInNewTab(const String& href);
};
}
}


#endif // RMLUI_XMLNODEHANDLERANCHOR_H
