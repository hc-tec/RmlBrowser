//
// Created by titto on 2023/1/2.
//

#ifndef RMLUI_JSDOCUMENTELEMENT_H
#define RMLUI_JSDOCUMENTELEMENT_H

#include "RmlUi/Core/ElementDocument.h"
#include <map>

namespace Rml {
namespace Script {


typedef Pair< String, const Property* > NamedProperty;
typedef Vector< NamedProperty > NamedPropertyList;

struct UnitProperty {
	const PropertySource* source;
    NamedPropertyList list;
};

struct ElementProperty {
	String inherit;
    Vector<UnitProperty> list;
};

typedef Vector<ElementProperty> ElementPropertyListWithInherit;

class JsDocumentElement : public Rml::ElementDocument
{
public:
	class Delegate {
	public:
		virtual void OnUpdate(const ElementPropertyListWithInherit& property_inherit) = 0;
	};
    JsDocumentElement(const String& tag);
    void LoadInlineScript(const String& content, const String& source_path, int source_line) override;
    void LoadExternalScript(const String& source_path) override;

    ElementPropertyListWithInherit GetElementProperties(Element* source);

	void set_source_element(Element* source);
    void set_delegate(Delegate* delegate);
protected:
	void OnRender() override;
	void OnUpdate() override;

private:

    void RenderSourceElement();
    static void BuildElementProperties(ElementPropertyListWithInherit& property_inherit, Element* element, Element* primary_element);

    Delegate* delegate_;
    Element* source_element;
};

}
}

#endif // RMLUI_JSDOCUMENTELEMENT_H
