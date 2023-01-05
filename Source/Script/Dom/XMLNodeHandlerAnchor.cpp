//
// Created by titto on 2023/1/5.
//

#include "XMLNodeHandlerAnchor.h"

#include "RmlUi/Core/Factory.h"
#include "RmlUi/Core/Element.h"

Rml::Element* Rml::Script::XMLNodeHandlerAnchor::ElementStart(Rml::XMLParser* parser, const Rml::String& name, const Rml::XMLAttributes& attributes)
{
    ElementPtr element = Factory::InstanceElement(parser->GetParseFrame()->element, name, name, attributes);
	// Add click event listener
	element->AddEventListener(EventId::Click, this);
    // Add the Select element into the document
    Element* result = parser->GetParseFrame()->element->AppendChild(std::move(element));
	return result;
}

void Rml::Script::XMLNodeHandlerAnchor::ProcessEvent(Rml::Event& event) {
    if (event.GetId() == EventId::Click) {
		Element* element = event.GetCurrentElement();
		String href = element->GetAttribute<String>("href", "");
		if (href.empty()) return;
		String target = element->GetAttribute<String>("target", "_self");
		if (target == "_self") {
            OpenInCurrentTab(href);
		} else if (target == "_blank") {
            OpenInNewTab(href);
		}
	}
}

void Rml::Script::XMLNodeHandlerAnchor::OpenInCurrentTab(const Rml::String& href) {

}

void Rml::Script::XMLNodeHandlerAnchor::OpenInNewTab(const Rml::String& href) {

}
