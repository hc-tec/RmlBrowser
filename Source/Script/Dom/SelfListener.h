//
// Created by titto on 2023/1/1.
//

#ifndef RMLUI_SELFLISTENER_H
#define RMLUI_SELFLISTENER_H

#include "RmlUi/Core/EventListener.h"

namespace Rml {

namespace Script {

class SelfListener : public EventListener {
	public:
        SelfListener(const std::function<void(Event*)>& callback);
		void ProcessEvent(Event& event) override;

	private:
        std::function<void(Event*)> callback_;
};
}

}


#endif // RMLUI_SELFLISTENER_H
