//
// Created by titto on 2023/1/1.
//

#ifndef RMLUI_SCRIPT_EVENT_H
#define RMLUI_SCRIPT_EVENT_H

#include "../quickjsppExtend.h"

namespace Rml {

namespace Script {

class Event {
public:
    static void Glue(qjs::Context::Module& m);
};

}

}


#endif // RMLUI_SCRIPT_EVENT_H
