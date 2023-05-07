//
// Created by titto on 2023/4/27.
//

#ifndef RMLUI_WORKERSCHEDULER_H
#define RMLUI_WORKERSCHEDULER_H

#include <co/co.h>

namespace Rml {

class WorkerScheduler {
public:
	co::Scheduler* Get();

private:
	void NextScheduler();
	co::Scheduler* s_{};
};

}

#endif // RMLUI_WORKERSCHEDULER_H
