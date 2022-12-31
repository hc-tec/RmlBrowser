//
// Created by titto on 2022/12/30.
//

#include "Element.h"

#include "RmlUi/Core/Element.h"
#include "Ownership.h"

namespace Rml {

namespace Script {

void Element::Glue(qjs::Context::Module& m) {
    m.class_<Rml::Element>("Element")
//        .fun<&MyClass::member_variable>("member_variable")
        .fun<&Rml::Element::GetId>("getId")
		.fun<&Rml::Element::SetInnerRML>("setInnerRML")

		.fun<&Rml::Element::GetInnerRML>("getInnerRML")
        .fun<>("appendChild", [](Rml::Element* _this, Rml::Element* child) {
            Log::Message(Log::LT_DEBUG, "child: %x", child);
			return _this->AppendChild(GetOwnership()->GetOwner(child), true);
        });

//	m.class_<Rml::ElementPtr>("ElementPtr");
//        .fun<&Rml::Element::GetId>("getId");
}

}

}