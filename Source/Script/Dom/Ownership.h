//
// Created by titto on 2022/12/31.
//

#ifndef RMLUI_OWNERSHIP_H
#define RMLUI_OWNERSHIP_H

#include "RmlUi/Core/Types.h"
#include <unordered_map>

namespace Rml {

namespace Script {

class BaseOwnership {};

template <typename T, typename UT>
class Ownership : public BaseOwnership {
public:
    T* ShiftOwner(UT ele_ptr) {
        T* ele = ele_ptr.get();
        owner_map_[ele] = std::move(ele_ptr);
        return ele;
	}
	UT GetOwner(T* ele) {
        auto it = owner_map_.find(ele);
        if (it == owner_map_.end()) return std::move(UT());
        return std::move(it->second);
	}

private:
	UnorderedMap<T*, UT> owner_map_;

};

static UnorderedMap<String, UniquePtr<BaseOwnership>> instance;

template <typename T, typename UT = UniquePtr<T>>
inline Ownership<T, UT>* GetOwnership() {
	const char* type_name = typeid(T).name();
	auto it = instance.find(type_name);
	if (it == instance.end()) {
		instance[type_name] = std::move(MakeUnique<Ownership<T, UT>>());
		it = instance.find(type_name);
	}
    return static_cast<Ownership<T, UT>*>(it->second.get());
}

inline void ClearAllOwner() {
	instance.clear();
}

}

}


#endif // RMLUI_OWNERSHIP_H
