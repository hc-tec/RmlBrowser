//
// Created by titto on 2023/4/26.
//

#ifndef RMLUI_XMLNODEHANDLERIMG_H
#define RMLUI_XMLNODEHANDLERIMG_H

#include <Core/XMLNodeHandlerDefault.h>

namespace Rml {

namespace Script {

class XMLNodeHandlerImg : public Rml::XMLNodeHandlerDefault {
public:
    Element* ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) override;
};

}
}

#endif // RMLUI_XMLNODEHANDLERIMG_H
