//
// Created by titto on 2023/5/7.
//

#ifndef RMLUI_CUSTOMCOMPONENT_H
#define RMLUI_CUSTOMCOMPONENT_H

#include "XMLNodeHandlerCustomComponent.h"

namespace Rml {

namespace Script {

class CustomComponent {
public:
    explicit CustomComponent(String name);

    CustomComponent* SetRml(String rml);

    CustomComponent* SetRcss(String rcss);

    CustomComponent* SetScript(String script);

	void Register();

private:
    String name_;
	CustomComponentAssert assert_;
};

}
}

#endif // RMLUI_CUSTOMCOMPONENT_H
