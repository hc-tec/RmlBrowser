//
// Created by titto on 2022/12/31.
//

#include "Ownership.h"

#include "RmlUi/Core/ElementText.h"

namespace Rml {

namespace Script {

	Element* Ownership::ShiftOwner(ElementPtr ele_ptr) {
        Element* ele = ele_ptr.get();
		owner_map_.emplace(ele, std::move(ele_ptr));
		return ele;
	}

	ElementPtr Ownership::GetOwner(Rml::Element* ele) {
		auto it = owner_map_.find(ele);
		if (it == owner_map_.end()) return std::move(ElementPtr(new ElementText("div")));
		return std::move(it->second);
	}


}

}