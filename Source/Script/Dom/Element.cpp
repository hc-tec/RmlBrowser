//
// Created by titto on 2022/12/30.
//

#include "Element.h"

#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/Property.h"
#include "RmlUi/Core/StyleSheet.h"
#include "RmlUi/Core/Elements/ElementFormControlInput.h"
#include "Ownership.h"
#include "SelfListener.h"

namespace Rml {

namespace Script {

void Element::Glue(qjs::Context::Module& m) {

    m.class_<Rml::StyleSheet>("StyleSheet");
//        .fun<&Rml::StyleSheet::MergeStyleSheet>("MergeStyleSheet");

    m.class_<Rml::Property>("Property")
        .fun<&Rml::Property::ToString>("toString");

    m.class_<Rml::Script::SelfListener>("EventListener");

    m.class_<Rml::Element>("Element")
        .fun<&Rml::Element::GetId>("getId")
        .fun<&Rml::Element::SetId>("setId")
        .fun<&Rml::Element::GetElementById>("getElementById")
        .fun<&Rml::Element::_GetElementsByTagName>("getElementsByTagName")
        .fun<&Rml::Element::_GetElementsByClassName>("getElementsByClassName")
        .fun<&Rml::Element::GetTagName>("getTagName")
        .fun<&Rml::Element::SetClass>("setClass")
        .fun<&Rml::Element::IsClassSet>("isClassSet")
        .fun<&Rml::Element::SetClassNames>("setClassNames")
        .fun<&Rml::Element::GetClassNames>("getClassNames")
        .fun<&Rml::Element::GetStyleSheet>("getStyleSheet")
        .fun<&Rml::Element::GetAddress>("getAddress")
        .fun<&Rml::Element::IsVisible>("isVisible")
        .fun<&Rml::Element::GetZIndex>("getZIndex")
        .fun<&Rml::Element::_SetProperty>("setProperty")
        .fun<&Rml::Element::_RemoveProperty>("removeProperty")
        .fun<&Rml::Element::_GetProperty>("getProperty")
        .fun<&Rml::Element::_GetAttribute>("getAttribute")
        .fun<&Rml::Element::_SetAttribute>("setAttribute")
        .fun<&Rml::Element::HasAttribute>("hasAttribute")
        .fun<&Rml::Element::RemoveAttribute>("removeAttribute")
        .fun<&Rml::Element::GetParentNode>("getParentNode")
        .fun<&Rml::Element::GetNextSibling>("getNextSibling")
        .fun<&Rml::Element::GetPreviousSibling>("getPreviousSibling")
        .fun<&Rml::Element::GetFirstChild>("getFirstChild")
        .fun<&Rml::Element::GetLastChild>("getLastChild")
        .fun<&Rml::Element::GetNumChildren>("getNumChildren")
        .fun<&Rml::Element::Focus>("focus")
        .fun<&Rml::Element::Blur>("blur")
        .fun<&Rml::Element::Click>("click")
        .fun<&Rml::Element::_ScrollIntoView>("scrollIntoView")
//		.fun<>("setAttribute", [](Rml::Element* _this, const String& name, const String& value) {
//			_this->SetAttribute(name, value);
//		})
        .fun<>("appendChild", [](Rml::Element* _this, Rml::Element* child) {
			return _this->AppendChild(GetOwnership<Rml::Element, ElementPtr>()->GetOwner(child), true);
        })
        .fun<>("removeChild", [](Rml::Element* _this, Rml::Element* child) {
			_this->RemoveChild(child);
			GetOwnership<Rml::Element>()->GetOwner(child).release();
        })
        .fun<>("addEventListener", [](Rml::Element* _this, const String& event, const std::function<void(Event*)>& callback) {
			UniquePtr<SelfListener> listener_ptr = MakeUnique<SelfListener>(event, callback);
			SelfListener* listener = listener_ptr.get();
            GetOwnership<SelfListener>()->ShiftOwner(std::move(listener_ptr));
            _this->AddEventListener(event, listener);
			return listener;
		})
		.fun<>("removeEventListener", [](Rml::Element* _this, SelfListener* listener) {
            _this->RemoveEventListener(listener->GetEvent(), listener);
			GetOwnership<SelfListener>()->GetOwner(listener).release();
		})
//		.fun("setValue", [](Rml::Element* _this, const String& value){
//			_this->GetContext();
//		})
        .property<&Rml::Element::_GetInnerRML, &Rml::Element::SetInnerRML>("innerRML");

        m.class_<Rml::ElementFormControl>("ElementFormControl")
            .base<Rml::Element>()
            .property<&Rml::ElementFormControl::GetValue, &Rml::ElementFormControl::SetValue>("value");

        m.class_<Rml::ElementFormControlInput>("ElementFormControlInput")
            .base<Rml::ElementFormControl>();

		m.function("ToElementFormControl", [](Rml::Element* el) -> ElementFormControl* {
			return reinterpret_cast<ElementFormControl*>(el);
		});

}

}

}