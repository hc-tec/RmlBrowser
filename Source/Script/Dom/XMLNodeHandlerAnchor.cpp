//
// Created by titto on 2023/1/5.
//

#include "XMLNodeHandlerAnchor.h"
#include "Ownership.h"
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/Factory.h"
#include "RmlUi/Core/SystemInterface.h"
#include "RmlUi/Core/Utils.h"
#include "SelfListener.h"

namespace Rml {

namespace Script {


void ProcessEvent(Rml::Event* event) {
    if (event->GetId() == EventId::Click) {
        Element* element = event->GetCurrentElement();
        Context* context = element->GetContext();
        String href = element->GetAttribute<String>("href", "");
        if (href.empty()) return;
        String target = element->GetAttribute<String>("target", "_self");
        if (target == "_self") {
            OpenInCurrentTab(element->GetOwnerDocument(), href);
        } else if (target == "_blank") {
            OpenInNewTab(element->GetOwnerDocument(), href);
        }
    } else if (event->GetId() == EventId::Unload) {
        Element* element = event->GetCurrentElement();
//        element->RemoveEventListener(EventId::Click, this);
//        element->RemoveEventListener(EventId::Unload, this);
    }
}


Rml::Element* XMLNodeHandlerAnchor::ElementStart(Rml::XMLParser* parser, const Rml::String& name, const Rml::XMLAttributes& attributes)
{
    ElementPtr element = Factory::InstanceElement(nullptr, name, name, attributes);
    // Add click event listener
	UniquePtr<SelfListener> listener = MakeUnique<SelfListener>("click", &ProcessEvent);
    element->AddEventListener(EventId::Click, listener.get());
	GetOwnershipMgr<SelfListener>()->ShiftOwner(std::move(listener));
    // Add the Select element into the document
    Element* result = parser->GetParseFrame()->element->AppendChild(std::move(element));
    result->SetAttributes(attributes);
    return result;
}

void OpenInCurrentTab(Rml::ElementDocument* document, const Rml::String& href) {
    Log::Message(Log::LT_DEBUG, "Open in Current Tab");
    String url = document->GetSourceURL();
	AnchorOpenInCurrentTabCallback(document, URL(Absolutepath(href, url)));
}

void OpenInNewTab(Rml::ElementDocument* document, const Rml::String& href) {
    Log::Message(Log::LT_DEBUG, "Open in New Tab");
    String url = document->GetSourceURL();
    AnchorOpenInNewTabCallback(document, URL(Absolutepath(href, url)));
}

bool XMLNodeHandlerAnchor::ElementEnd(XMLParser* parser, const String& name) { return true; }

bool XMLNodeHandlerAnchor::ElementData(XMLParser* parser, const String& data, XMLDataType type) {
    return Factory::InstanceElementText(parser->GetParseFrame()->element, data);
}

}

}
