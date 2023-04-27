//
// Created by titto on 2023/4/27.
//

#ifndef RMLUI_RENDERSCHEDULER_H
#define RMLUI_RENDERSCHEDULER_H

#include <co/co.h>

namespace Rml {

class RenderScheduler {
public:
    RenderScheduler();
    co::Scheduler* Get();

private:
    co::Scheduler* s_{};
};

}
#endif // RMLUI_RENDERSCHEDULER_H
