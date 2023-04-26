//
// Created by titto on 2023/4/20.
//

#include "Glue.h"

#include "NetHttp.h"



namespace Rml {
namespace Script {

void NetGlue(qjs::Context* context) {
    qjs::Context::Module& m = context->addModule("net");
    m.class_<NetHttp>("Http")
        .constructor<>()
        .fun<&NetHttp::Get>("get")
        .fun<&NetHttp::Post>("post");

}

}
}
