//
// Created by titto on 2023/1/1.
//

#include "SelfListener.h"

namespace Rml {

namespace Script {

SelfListener::SelfListener(const std::function<void(Event*)>& callback)
    : callback_(callback) {}

void SelfListener::ProcessEvent(Event& event) {
	callback_(&event);
}


}

}