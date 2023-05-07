//
// Created by titto on 2023/1/5.
//

#ifndef RMLUI_XMLNODEHANDLERANCHOR_H
#define RMLUI_XMLNODEHANDLERANCHOR_H

#include <utility>

#include "RmlUi/Core/XMLNodeHandler.h"
#include "RmlUi/Core/EventListener.h"
#include "RmlUi/Core/URL.h"
#include "RmlUi/Core/ElementDocument.h"

namespace Rml {

namespace Script {

extern void AnchorOpenInCurrentTabCallback(Rml::ElementDocument* document, const URL& url);
extern void AnchorOpenInNewTabCallback(Rml::ElementDocument* document, const URL& url);

class XMLNodeHandlerAnchor : public Rml::XMLNodeHandler {
public:
    Element* ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) override;
	bool ElementEnd(XMLParser* parser, const String& name) override;
	bool ElementData(XMLParser* parser, const String& data, XMLDataType type) override;

};

void OpenInCurrentTab(Rml::ElementDocument* document, const String& href);
void OpenInNewTab(Rml::ElementDocument* document, const String& href);

}
}


#endif // RMLUI_XMLNODEHANDLERANCHOR_H
