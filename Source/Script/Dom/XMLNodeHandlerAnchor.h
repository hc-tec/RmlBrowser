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

extern void AnchorOpenInCurrentTabCallback(Context* context, const URL& url);
extern void AnchorOpenInNewTabCallback(Context* context, const URL& url);

class XMLNodeHandlerAnchor : public Rml::XMLNodeHandler {
public:
    Element* ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) override;
	bool ElementEnd(XMLParser* parser, const String& name) override;
	bool ElementData(XMLParser* parser, const String& data, XMLDataType type) override;

};

void OpenInCurrentTab(Context* context, const String& href);
void OpenInNewTab(Context* context, const String& href);

}
}


#endif // RMLUI_XMLNODEHANDLERANCHOR_H
