//
// Created by titto on 2023/1/1.
//

#include "SelfListener.h"
#include <quickjspp.hpp>
#include <utility>
#include "log/logging.h"
namespace Rml {

namespace Script {

SelfListener::SelfListener(String  event, const std::function<void(Event*)>& callback)
    : event_(std::move(event)), callback_(callback) {}

void SelfListener::ProcessEvent(Event& event) {
	try
	{
		callback_(&event);
	} catch (qjs::exception e) {
        auto exc = e.get();
		LOG(ERROR) << (std::string) exc;
        if((bool) exc["stack"])
            LOG(ERROR) << (std::string) exc["stack"];
    }
}


}

}