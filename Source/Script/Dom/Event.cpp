//
// Created by titto on 2023/1/1.
//

#include "Event.h"

#include "RmlUi/Core/Event.h"
#include "RmlUi/Core/Element.h"

namespace Rml {

namespace Script {

void Event::Glue(qjs::Context::Module& m) {

	m.class_<Rml::SmallUnorderedMap< String, Variant >>("Dictionary");
//        .fun<>("find", [](Rml::SmallUnorderedMap< String, Variant >* dictionary, const String& param){
//            auto it = dictionary->find(param);
//            if (it == dictionary->end()) return VARIANT_NONE.get()->Get(0);
//            return it->second.Get(0);
//		});

    m.class_<Rml::Event>("Event")
        .fun<&Rml::Event::GetId>("getId")
        .fun<&Rml::Event::GetCurrentElement>("getCurrentElement")
        .fun<&Rml::Event::GetTargetElement>("getTargetElement")
        .fun<&Rml::Event::GetType>("getType")
        .fun<&Rml::Event::StopPropagation>("stopPropagation")
        .fun<&Rml::Event::IsPropagating>("isPropagating")
        .fun<&Rml::Event::GetParameters>("getParameters");
}

}

}