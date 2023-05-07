//
// Created by titto on 2023/4/27.
//

#ifndef RMLUI_RENDERSCHEDULER_H
#define RMLUI_RENDERSCHEDULER_H

#include <co/co.h>

namespace Rml {

class RenderScheduler {
public:
    static co::Scheduler* Get() {
        static co::Scheduler* s_ = co::schedulers()[0];
		return s_;
	}

private:

};

}
#endif // RMLUI_RENDERSCHEDULER_H
