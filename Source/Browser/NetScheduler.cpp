//
// Created by titto on 2023/4/27.
//

#include "NetScheduler.h"

namespace Rml {

co::Scheduler* NetScheduler::Get()
{
	NextScheduler();
	return s_;
}

void NetScheduler::NextScheduler() {
    s_ = co::next_scheduler();
    if (s_ == co::schedulers()[0]) NextScheduler();
}

}