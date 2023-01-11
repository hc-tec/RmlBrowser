//
// Created by titto on 2022/12/31.
//

#ifndef RMLUI_OWNERSHIP_H
#define RMLUI_OWNERSHIP_H

#include <any>

#include "RmlUi/Core/Types.h"
#include <set>

namespace Rml {

namespace Script {

template <typename T>
struct unique_traits {
	using uni_ptr = UniquePtr<T>;
};

template <>
struct unique_traits<Rml::Element> {
    using uni_ptr = ElementPtr;
};

class OwnershipObserver {
public:
    virtual void OnOwnerShift(std::any ptr) {};
};


extern Vector<OwnershipObserver*> ownershipObservers;

inline void RegisterOwnershipObserver(OwnershipObserver* observer) {
    ownershipObservers.push_back(observer);
}

inline void UnRegisterOwnershipObserver(OwnershipObserver* observer) {
	ownershipObservers.erase(std::find(ownershipObservers.begin(), ownershipObservers.end(), observer));
}

class BaseOwnership {};

template <typename T, typename UT>
class Ownership : public BaseOwnership {
public:
    T* ShiftOwner(UT ele_ptr) {
        T* ele = ele_ptr.get();
        owner_map_[ele] = std::move(ele_ptr);
        NotifyOwnerShift(ele);
        return ele;
	}
	UT GetOwner(T* ele) {
        auto it = owner_map_.find(ele);
        if (it == owner_map_.end()) return std::move(UT());
        return std::move(it->second);
	}

    virtual void NotifyOwnerShift(std::any ptr) {
		for(auto obs : ownershipObservers) {
			obs->OnOwnerShift(ptr);
		}
	};
private:
	UnorderedMap<T*, UT> owner_map_;

};

extern UnorderedMap<String, UniquePtr<BaseOwnership>> instance;

template <typename T>
inline Ownership<T, typename unique_traits<T>::uni_ptr>* GetOwnershipMgr() {
    using UT = typename unique_traits<T>::uni_ptr;
	const char* type_name = typeid(T).name();
	auto it = instance.find(type_name);
	if (it == instance.end()) {
		instance[type_name] = std::move(MakeUnique<Ownership<T, UT>>());
		it = instance.find(type_name);
	}
    return static_cast<Ownership<T, UT>*>(it->second.get());
}

template <typename T>
inline Ownership<T, typename unique_traits<T>::uni_ptr>* GetOwnershipMgr(T val) {
    using UT = typename unique_traits<T>::uni_ptr;
    const char* type_name = typeid(T).name();
    auto it = instance.find(type_name);
    if (it == instance.end()) {
        instance[type_name] = std::move(MakeUnique<Ownership<T, UT>>());
        it = instance.find(type_name);
    }
    return static_cast<Ownership<T, UT>*>(it->second.get());
}

extern void ClearOwners(Vector<std::any> vec);

}

}


#endif // RMLUI_OWNERSHIP_H
