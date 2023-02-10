//
// Created by titto on 2023/1/2.
//

#include "JsDocumentElementInstancer.h"
#include "RmlUi/Core/XMLParser.h"
#include "RmlUi/Core/SystemInterface.h"

#include "RmlUi/Core/URL.h"
#include "JsDocumentElement.h"
#include "../RunTime.h"


namespace Rml {
namespace Script {

static String Absolutepath(const String& source, const String& base)
{
    String joined_path;
    Rml::GetSystemInterface()->JoinPath(joined_path, StringUtilities::Replace(base, '|', ':'), StringUtilities::Replace(source, '|', ':'));
    return joined_path;//StringUtilities::Replace(joined_path, ':', '|');
}

static DocumentHeader::Resource MakeInlineResource(XMLParser* parser, const String& data)
{
    DocumentHeader::Resource resource;
    resource.is_inline = true;
    resource.content = data;
    resource.path = parser->GetSourceURL().GetURL();
    resource.line = parser->GetLineNumberOpenTag();
    return resource;
}

static DocumentHeader::Resource MakeExternalResource(XMLParser* parser, const String& path)
{
    DocumentHeader::Resource resource;
    resource.is_inline = false;
    URL url(path);
    if (url.GetProtocol().substr(0, 4) == "file")
    {
        resource.path = Absolutepath(path, parser->GetSourceURL().GetURL());
    } else {
        resource.path = path;
    }
    return resource;
}

/// Instances an element given the tag name and attributes.
/// @param[in] parent The element the new element is destined to be parented to.
/// @param[in] tag The tag of the element to instance.
/// @param[in] attributes Dictionary of attributes.
ElementPtr JsDocumentElementInstancer::InstanceElement(Element* /*parent*/, const String& tag, const XMLAttributes& /*attributes*/)
{
    return ElementPtr(new JsDocumentElement(tag));
}
/// Releases an element instanced by this instancer.
/// @param[in] element The element to release.
void JsDocumentElementInstancer::ReleaseElement(Element* element)
{
    delete element;
}

Element* JsDocumentElementInstancer::ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) {
    Element* element = parser->GetParseFrame()->element;
    // Check if its an external string
    String src = Get<String>(attributes, "src", "");
    if (src.size() > 0)
    {
        scripts_.push_back(MakeExternalResource(parser, src));
    }
	return element;
}

bool JsDocumentElementInstancer::ElementEnd(XMLParser* parser, const String& name) { return true; }

bool JsDocumentElementInstancer::ElementData(XMLParser* parser, const String& data, XMLDataType type) {
    if (data.empty()) return true;
	scripts_.push_back(MakeInlineResource(parser, data));
	return true;
}

}
}