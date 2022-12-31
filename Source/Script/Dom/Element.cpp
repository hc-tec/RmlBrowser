//
// Created by titto on 2022/12/30.
//

#include "Element.h"

#include "RmlUi/Core/Element.h"

namespace Rml {

namespace Script {

void Element::Glue(qjs::Context::Module& m) {
    m.class_<Rml::Element>("Element")
//        .fun<&MyClass::member_variable>("member_variable")
        .fun<&Rml::Element::GetId>("getId");

	m.class_<Rml::ElementPtr>("ElementPtr");
//        .fun<&Rml::Element::GetId>("getId");
}

}

}