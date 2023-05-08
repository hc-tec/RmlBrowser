//
// Created by titto on 2023/5/7.
//

#include "CustomComponent.h"

namespace Rml {

namespace Script {

CustomComponent::CustomComponent(String name) : name_(std::move(name)) {}

CustomComponent* CustomComponent::SetRml(String rml) {
	assert_.rml = std::move(rml);
	return this;
}

CustomComponent* CustomComponent::SetRcss(String rcss) {
    assert_.rcss = std::move(rcss);
    return this;
}

CustomComponent* CustomComponent::SetScript(String script) {
    assert_.script = std::move(script);
    return this;
}

void CustomComponent::Register() {
	XMLNodeHandlerCustomComponent::RegisterCustomComponent(name_, assert_);
}

}
}