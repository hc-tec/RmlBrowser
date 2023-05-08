//
// Created by titto on 2023/5/7.
//

#include "XMLNodeHandlerSlot.h"
#include "XMLNodeHandlerCustomComponent.h"

namespace Rml {

namespace Script {

Element* XMLNodeHandlerSlot::ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes)
{
//    Element* res = XMLNodeHandlerDefault::ElementStart(parser, "div", attributes);
//
//	return res;

    Element* p = parser->GetParseFrame()->element;
	while (p != nullptr && p->GetId().compare(0, 3, "rue")) {
		p = p->GetParentNode();
	}
    String id = p->GetId();
    String slot_name = "default";
	if (attributes.find("name") != attributes.end())
	{
        slot_name = attributes.find("name")->second.Get<String>("");
	}
	String slot = id + "__" + slot_name;
    XMLNodeHandlerDefault::ElementData(parser, XMLNodeHandlerCustomComponent::GetSlot(slot), XMLDataType::InnerXML);
	return nullptr;
}

bool XMLNodeHandlerSlot::ElementData(XMLParser* parser, const String& data, XMLDataType type)
{
//    return XMLNodeHandlerDefault::ElementData(parser, XMLNodeHandlerCustomComponent::GetSlot(), XMLDataType::InnerXML);;
    return true;
}

}
}
