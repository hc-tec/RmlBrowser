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

    void Get(String url, qjs::Value params, qjs::Value headers, const qjs::Value& options);

    void Post(String url, qjs::Value params, qjs::Value headers, const qjs::Value& options);



private:
    void Request(String url, net::Method method, qjs::Value params, qjs::Value headers, const qjs::Value& options);

	void ParseHeader(net::RequestParams& p, qjs::Value headers);

	void GenerateResponse(qjs::Value& ret, net::HttpResponseInfo* res);

private:
	UniquePtr<NetHttpImpl> impl_;

};

}
}

#endif // RMLUI_NETHTTP_H
