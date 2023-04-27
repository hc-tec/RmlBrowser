//
// Created by titto on 2023/4/27.
//

#include "RenderScheduler.h"

namespace Rml {

RenderScheduler::RenderScheduler()
    : s_(co::schedulers()[0]) {}

co::Scheduler* RenderScheduler::Get()
{
    return s_;
}

}