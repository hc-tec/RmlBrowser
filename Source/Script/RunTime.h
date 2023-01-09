//
// Created by titto on 2022/12/30.
//

#ifndef RMLUI_RUNTIME_H
#define RMLUI_RUNTIME_H

#include "quickjspp.hpp"
#include <iostream>
#include <co/co/mutex.h>

namespace Rml {

namespace Script {

static qjs::Runtime* runtime;


inline qjs::Runtime* GetRunTime() {
	if (runtime == nullptr)
	{
		runtime = new qjs::Runtime();
	}
	return runtime;
}

static qjs::Context* context;

inline qjs::Context* GetContext(bool fresh = false) {
	if (context == nullptr)
	{
		context = new qjs::Context(*GetRunTime());
	} else if (fresh) {
        delete context;
        context = new qjs::Context(*GetRunTime());
    }
    return context;
}

}

}


#endif // RMLUI_RUNTIME_H
