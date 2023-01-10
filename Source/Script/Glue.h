//
// Created by titto on 2022/12/30.
//

#ifndef RMLUI_GLUE_H
#define RMLUI_GLUE_H

#include "quickjspp.hpp"
#include "RmlUi/Core/Types.h"

namespace Rml {

typedef void (*GlueFunc)(qjs::Context* context);
static Vector<GlueFunc> GLUE_FUNC_LIST;
void RegisterGlueFunc(GlueFunc func);

namespace Script {

void Glue(qjs::Context* context);

}

}


#endif // RMLUI_GLUE_H
