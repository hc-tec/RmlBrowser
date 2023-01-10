//
// Created by titto on 2022/12/30.
//

#ifndef RMLUI_RUNTIME_H
#define RMLUI_RUNTIME_H

#include "quickjspp.hpp"
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

static qjs::Context* js_context;

inline qjs::Context* GetContext(bool fresh = false) {
	if (js_context == nullptr)
	{
        js_context = new qjs::Context(*GetRunTime());
	} else if (fresh) {
        delete js_context;
        js_context = new qjs::Context(*GetRunTime());
    }
    return js_context;
}

}

}


#endif // RMLUI_RUNTIME_H
