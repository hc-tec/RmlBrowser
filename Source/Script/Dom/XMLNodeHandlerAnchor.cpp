//
// Created by titto on 2023/1/5.
//

#include "XMLNodeHandlerAnchor.h"

#include "RmlUi/Core/Factory.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/SystemInterface.h"

namespace Rml {

namespace Script {

static String Absolutepath(const String& source, const String& base)
{
    String joined_path;
    Rml::GetSystemInterface()->JoinPath(joined_path, StringUtilities::Replace(base, '|', ':'), StringUtilities::Replace(source, '|', ':'));
    return StringUtilities::Replace(joined_path, ':', '|');
}


Rml::Element* XMLNodeHandlerAnchor::ElementStart(Rml::XMLParser* parser, const Rml::String& name, const Rml::XMLAttributes& attributes)
{
    ElementPtr element = Factory::InstanceElement(parser->GetParseFrame()->element, name, name, attributes);
    // Add click event listener
    element->AddEventListener(EventId::Click, this);
    // Add the Select element into the document
    Element* result = parser->GetParseFrame()->element->AppendChild(std::move(element));
    return result;
}

void XMLNodeHandlerAnchor::ProcessEvent(Rml::Event& event) {
    if (event.GetId() == EventId::Click) {
        Element* element = event.GetCurrentElement();
        Context* context = element->GetContext();
        String href = element->GetAttribute<String>("href", "");
        if (href.empty()) return;
        String target = element->GetAttribute<String>("target", "_self");
        if (target == "_self") {
            OpenInCurrentTab(context, href);
        } else if (target == "_blank") {
            OpenInNewTab(context, href);
        }
    }
}

void XMLNodeHandlerAnchor::OpenInCurrentTab(Context* context, const Rml::String& href) {
    Log::Message(Log::LT_DEBUG, "Open in Current Tab");
	String url = context->GetDocument(context->GetName())->GetSourceURL();
	AnchorOpenInCurrentTabCallback(context, URL(Absolutepath(href, url)));
}

void XMLNodeHandlerAnchor::OpenInNewTab(Context* context, const Rml::String& href) {
    Log::Message(Log::LT_DEBUG, "Open in New Tab");
    String url = context->GetDocument(context->GetName())->GetSourceURL();
    AnchorOpenInNewTabCallback(context, URL(Absolutepath(href, url)));
}

bool XMLNodeHandlerAnchor::ElementEnd(XMLParser* parser, const String& name) { return true; }

bool XMLNodeHandlerAnchor::ElementData(XMLParser* parser, const String& data, XMLDataType type) {
    return Factory::InstanceElementText(parser->GetParseFrame()->element, data);
}

}

}