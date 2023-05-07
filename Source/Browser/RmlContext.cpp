//
// Created by titto on 2023/5/5.
//

#include "RmlContext.h"

namespace Rml {
namespace Browser {

RmlContext::~RmlContext() {
    Backend::UnRegisterContext(Get());
    Rml::RemoveContext(BROWSER_ID);
}


}
}