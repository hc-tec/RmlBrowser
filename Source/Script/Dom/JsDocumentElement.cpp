//
// Created by titto on 2023/1/2.
//

#include "JsDocumentElement.h"
#include "RmlUi/Core/ElementUtilities.h"
#include "Browser/RmlContext.h"
#include "Geometry.h"
#include <iostream>

namespace Rml {

namespace Script {

JsDocumentElement::JsDocumentElement(const String& tag) : ElementDocument(tag),
		delegate_(nullptr),
		source_element(nullptr)
{
    Geometry::SetContext(Browser::RmlContext::Get());
}

void JsDocumentElement::LoadInlineScript(const String& context, const String& source_path, int source_line)
{

}

void JsDocumentElement::LoadExternalScript(const String& source_path)
{

}

void JsDocumentElement::OnRender() {
    if (source_element) {
		RenderSourceElement();
	}
}

void JsDocumentElement::RenderSourceElement()
{
    ElementUtilities::ApplyTransform(*source_element);

    for (int i = 0; i < source_element->GetNumBoxes(); i++)
    {
        Vector2f box_offset;
        const Box element_box = source_element->GetBox(i, box_offset);
        const Vector2f border_offset = box_offset + source_element->GetAbsoluteOffset(Box::BORDER);

        // Content area:
        Geometry::RenderBox(border_offset + element_box.GetPosition(Box::CONTENT), element_box.GetSize(), Colourb(158, 214, 237, 128));

        // Padding area:
        Geometry::RenderBox(border_offset + element_box.GetPosition(Box::PADDING), element_box.GetSize(Box::PADDING), border_offset + element_box.GetPosition(Box::CONTENT), element_box.GetSize(), Colourb(135, 122, 214, 128));

        // Border area:
        Geometry::RenderBox(border_offset + element_box.GetPosition(Box::BORDER), element_box.GetSize(Box::BORDER), border_offset + element_box.GetPosition(Box::PADDING), element_box.GetSize(Box::PADDING), Colourb(192,219,180, 128));

        // Border area:
        Geometry::RenderBox(border_offset + element_box.GetPosition(Box::MARGIN), element_box.GetSize(Box::MARGIN), border_offset + element_box.GetPosition(Box::BORDER), element_box.GetSize(Box::BORDER), Colourb(247,202,155, 128));
    }

}

void JsDocumentElement::set_source_element(Element* source) {
	source_element = source;
}

void JsDocumentElement::OnUpdate() {

}

ElementPropertyListWithInherit JsDocumentElement::GetElementProperties(Element* source) {
    ElementPropertyListWithInherit property_inherit;
    BuildElementProperties(property_inherit, source, source);
	return std::move(property_inherit);
}

void JsDocumentElement::BuildElementProperties(ElementPropertyListWithInherit& property_inherit, Element* element, Element* primary_element)
{

    NamedPropertyList property_list;

    for(auto it = element->IterateLocalProperties(); !it.AtEnd(); ++it)
    {
        PropertyId property_id = it.GetId();
        const String& property_name = it.GetName();
        const Property* prop = &it.GetProperty();

        // Check that this property isn't overridden or just not inherited.
        if (primary_element->GetProperty(property_id) != prop)
            continue;

        property_list.push_back(NamedProperty{ property_name, prop });
    }

    std::sort(property_list.begin(), property_list.end(),
              [](const NamedProperty& a, const NamedProperty& b) {
                if (a.second->source && !b.second->source) return false;
                if (!a.second->source && b.second->source) return true;
                if (a.second->specificity < b.second->specificity) return false;
                if (a.second->specificity > b.second->specificity) return true;
                if (a.second->definition && !b.second->definition) return false;
                if (!a.second->definition && b.second->definition) return true;
                const String& a_name = StyleSheetSpecification::GetPropertyName(a.second->definition->GetId());
                const String& b_name = StyleSheetSpecification::GetPropertyName(b.second->definition->GetId());
                return a_name < b_name;
              }
    );

    if (!property_list.empty())
    {
		ElementProperty property;

        // Print the 'inherited from ...' header if we're not the primary element.
        String inherit;
        if (element != primary_element)
        {
            inherit = element->GetAddress(false, false);
        } else {
			inherit = "";
		}
		property.inherit = std::move(inherit);

		UnitProperty unit;

        const PropertySource* previous_source = nullptr;

        for (auto& named_property : property_list)
		{
			auto& source = named_property.second->source;
			if (source.get() != previous_source)
			{
				if (!unit.list.empty())
				{
					property.list.push_back(unit);
				}
				unit.list.clear();
				unit.source = source.get();
				previous_source = source.get();
			}
			unit.source = source.get();
			unit.list.push_back(std::move(named_property));
		}
        property_inherit.push_back(std::move(property));
    }

    if (element->GetParentNode() != nullptr)
        BuildElementProperties(property_inherit, element->GetParentNode(), primary_element);
}

void JsDocumentElement::set_delegate(JsDocumentElement::Delegate* delegate) {
	delegate_ = delegate;
}


}

}