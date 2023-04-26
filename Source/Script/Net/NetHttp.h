//
// Created by titto on 2023/4/20.
//

#ifndef RMLUI_NETHTTP_H
#define RMLUI_NETHTTP_H

#include "Impl/NetHttpImpl.h"
#include "quickjspp.hpp"

namespace Rml {
namespace Script {

class NetHttp {
public:

    NetHttp();

    qjs::Value Get(String url, qjs::Value params, qjs::Value headers);

    qjs::Value Post(String url, qjs::Value params, qjs::Value headers);



private:
    qjs::Value Request(String url, net::Method method, qjs::Value params, qjs::Value headers);

private:
	UniquePtr<NetHttpImpl> impl_;

};

}
}

#endif // RMLUI_NETHTTP_H
