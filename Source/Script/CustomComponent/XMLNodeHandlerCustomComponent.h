//
// Created by titto on 2023/5/7.
//

#ifndef RMLUI_XMLNODEHANDLERCUSTOMCOMPONENT_H
#define RMLUI_XMLNODEHANDLERCUSTOMCOMPONENT_H

#include "Core/XMLNodeHandlerDefault.h"

#include "RmlUi/Core/ElementDocument.h"
#include "RmlUi/Core/Factory.h"

namespace Rml {

namespace Script {

struct CustomComponentAssert {
    String name;
    String rml;
    String rcss;
    String script;
};


class XMLNodeHandlerCustomComponent : public Rml::XMLNodeHandlerDefault {
public:

    Rml::Element* ElementStart(XMLParser* parser, const String& name, const XMLAttributes& attributes) override;

    bool ElementData(XMLParser* parser, const String& data, XMLDataType type) override;

	static void RegisterCustomComponent(String name, CustomComponentAssert assert);

	static void SetSlotData(String id, String rml);

	static String GetSlot(const String& id);

	static UnorderedMap<String, CustomComponentAssert>& GetComponentAssets();

	static Vector<CustomComponentAssert>& GetUseComponentAssets();

    static void ClearUseComponentAssets();

	static bool NeedParse();

	static Element* GetParseEl();

private:

	void ReplaceScript(CustomComponentAssert& assert, const String& id);

};


}
}


#endif // RMLUI_XMLNODEHANDLERCUSTOMCOMPONENT_H
