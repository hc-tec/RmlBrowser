//
// Created by titto on 2022/12/31.
//

#ifndef RMLUI_OWNERSHIP_H
#define RMLUI_OWNERSHIP_H

#include "RmlUi/Core/Types.h"
#include <unordered_map>

namespace Rml {

namespace Script {

class Ownership {
public:

    Element* ShiftOwner(ElementPtr ele_ptr);
	ElementPtr GetOwner(Rml::Element* ele);

private:
	UnorderedMap<Element*, ElementPtr> owner_map_;

};

static Ownership instance;
inline Ownership* GetOwnership() {
    return &instance;
}

}

}


#endif // RMLUI_OWNERSHIP_H
