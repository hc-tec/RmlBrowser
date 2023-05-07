//
// Created by titto on 2023/4/27.
//

#include "WorkerScheduler.h"

namespace Rml {

co::Scheduler* WorkerScheduler::Get()
{
	NextScheduler();
	return s_;
}

void WorkerScheduler::NextScheduler() {
    s_ = co::next_scheduler();
    if (s_ == co::schedulers()[0]) NextScheduler();
}

}