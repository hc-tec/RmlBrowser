//
// Created by titto on 2023/1/5.
//

#include "Tab.h"

namespace Rml {
namespace Browser {

Tab::Tab(const String& tab_id, const URL& url)
    : tab_id_(tab_id),
		url_(url) {}

}
}