//
// Created by titto on 2023/1/1.
//

#include "SelfListener.h"

#include <utility>

namespace Rml {

namespace Script {

SelfListener::SelfListener(String  event, const std::function<void(Event*)>& callback)
    : event_(std::move(event)), callback_(callback) {}

void SelfListener::ProcessEvent(Event& event) {
	callback_(&event);
}


}

}