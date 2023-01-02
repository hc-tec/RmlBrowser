//
// Created by titto on 2023/1/2.
//

#ifndef RMLUI_JSDOCUMENTELEMENTINSTANCER_H
#define RMLUI_JSDOCUMENTELEMENTINSTANCER_H

#include "RmlUi/Core/ElementInstancer.h"
#include "RmlUi/Core/XMLNodeHandler.h"

#include "../../Source/Core/DocumentHeader.h"

namespace Rml {
namespace Script {

class JsDocumentElementInstancer : public Rml::ElementInstancer, public Rml::XMLNodeHandler
{
    /// Instances an element given the tag name and attributes.
    /// @param[in] parent The element the new element is destined to be parented to.
    /// @param[in] tag The tag of the element to instance.
    /// @param[in] attributes Dictionary of attributes.
    ElementPtr InstanceElement(Element* parent, const String& tag, const XMLAttributes& attributes) override;
    /// Releases an element instanced by this instancer.
    /// @param[in] element The element to release.
    void ReleaseElement(Element* element) override;

public:

    const DocumentHeader::ResourceList& GetScripts() { return scripts_; }

	Element* ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) override;
	bool ElementEnd(XMLParser* parser, const String& name) override;
	bool ElementData(XMLParser* parser, const String& data, XMLDataType type) override;

    /// script source
    DocumentHeader::ResourceList scripts_;
};

}
}

#endif // RMLUI_JSDOCUMENTELEMENTINSTANCER_H
