//
// Created by titto on 2023/4/26.
//

#include "XMLNodeHandlerImg.h"
#include "RmlUi/Core/Element.h"

namespace Rml {

namespace Script {

Element* XMLNodeHandlerImg::ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes)
{
    Element* res = XMLNodeHandlerDefault::ElementStart(parser, name, attributes);
    res->SetAttributes(attributes);
    return res;
}

}
}