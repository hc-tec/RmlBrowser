//
// Created by titto on 2023/1/5.
//

#ifndef RMLUI_TAB_H
#define RMLUI_TAB_H

#include "RmlUi/Config/Config.h"
#include "RmlUi/Core/URL.h"

namespace Rml {
namespace Browser {

class Tab {
public:
	Tab(const String& tab_id, const URL& url);

private:
	String tab_id_;
	URL url_;
};

}
}


#endif // RMLUI_TAB_H
