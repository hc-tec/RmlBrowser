//
// Created by titto on 2022/12/31.
//

#include "Ownership.h"

#include "RmlUi/Core/Element.h"
#include "SelfListener.h"

namespace Rml {

namespace Script {

Vector<OwnershipObserver*> ownershipObservers;
UnorderedMap<String, UniquePtr<BaseOwnership>> instance;

void ClearOwners(Vector<std::any> vec) {
    for(auto el : vec) {
        if (el.type() == typeid(Rml::Element)) {
            Rml::Element* ptr = std::any_cast<Rml::Element*>(el);
            GetOwnershipMgr(ptr)->GetOwner(&ptr).reset();
        } else if (el.type() == typeid(Rml::Script::SelfListener))
        {
            Rml::Script::SelfListener* ptr = std::any_cast<Rml::Script::SelfListener*>(el);
            GetOwnershipMgr(ptr)->GetOwner(&ptr).reset();
        }
    }
}


}

}