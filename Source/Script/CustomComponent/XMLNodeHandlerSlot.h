//
// Created by titto on 2023/5/7.
//

#ifndef RMLUI_XMLNODEHANDLERSLOT_H
#define RMLUI_XMLNODEHANDLERSLOT_H

#include "Core/XMLNodeHandlerDefault.h"
#include "RmlUi/Core/ElementDocument.h"

namespace Rml {

namespace Script {

class XMLNodeHandlerSlot : public Rml::XMLNodeHandlerDefault {
public:
    Element* ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) override;
    //    bool ElementEnd(XMLParser* parser, const String& name) override;
    bool ElementData(XMLParser* parser, const String& data, XMLDataType type) override;

};

}
}

#endif // RMLUI_XMLNODEHANDLERSLOT_H
