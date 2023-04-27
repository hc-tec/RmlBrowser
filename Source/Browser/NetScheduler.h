//
// Created by titto on 2023/4/27.
//

#ifndef RMLUI_NETSCHEDULER_H
#define RMLUI_NETSCHEDULER_H

#include <co/co.h>

namespace Rml {

class NetScheduler {
public:
	co::Scheduler* Get();

private:
	void NextScheduler();
	co::Scheduler* s_{};
};

}

#endif // RMLUI_NETSCHEDULER_H
