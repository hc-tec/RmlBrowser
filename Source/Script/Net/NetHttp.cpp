//
// Created by titto on 2023/4/20.
//

#include "NetHttp.h"

#include <utility>
#include <json/json.h>

#include "Script/quickjsppExtend.h"

namespace Rml {
namespace Script {

NetHttp::NetHttp()
		: impl_(MakeUnique<NetHttpImpl>()) {}

qjs::Value NetHttp::Request(String url, net::Method method, qjs::Value params, qjs::Value headers)
{
	net::RequestParams p;
	p.request_info.method = method;
	p.request_info.url = std::move(url);
    p.request_info.body = std::make_shared<net::HttpRequestBufferBody>(
        "application/json;charset=UTF-8", params.toJSON());

	String h = headers.toJSON();
    Json::Reader reader;
    Json::Value obj;
    reader.parse(h, obj);

    net::HttpHeaders httpHeaders;

    Json::Value::Members members = obj.getMemberNames();
    for (const auto& member : members) {
        httpHeaders.PutHeaders(member, obj[member].asString());
	}

    p.request_info.headers = httpHeaders;
    auto res = impl_->Get(p);

    JSValue o = JS_NewObject(params.ctx);
	qjs::Value ret {params.ctx, JS_DupValue(params.ctx, o)};

	ret["code"] = res->status.code;
    ret["msg"] = res->status.description;
	ret["header"] = res->headers; // qjs::js_traits<net::HttpHeaders>::wrap(params.ctx, res->headers);

    std::string_view buf;
	const int bytes = 1024;
    res->body->Read(&buf, bytes);
    char buffer[bytes];
    memcpy(buffer, buf.data(), bytes);

    ret["data"] = String(buffer);
	return ret;
}

qjs::Value NetHttp::Get(String url, qjs::Value params, qjs::Value headers) {
	return Request(std::move(url), net::Method::GET, std::move(params), std::move(headers));
}

qjs::Value NetHttp::Post(String url, qjs::Value params, qjs::Value headers)
{
    return Request(std::move(url), net::Method::POST, std::move(params), std::move(headers));
}

}
}