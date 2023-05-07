//
// Created by titto on 2023/4/20.
//

#include "NetHttp.h"
#include "../../Browser/RenderScheduler.h"
#include "../../Browser/WorkerScheduler.h"
#include "Script/quickjsppExtend.h"
#include "quickjspp.hpp"
#include <json/json.h>
#include <utility>

namespace Rml {
namespace Script {

NetHttp::NetHttp()
		: impl_(MakeUnique<NetHttpImpl>()) {}

void NetHttp::Request(String url, net::Method method, qjs::Value params, qjs::Value headers, const qjs::Value& options)
{
	net::RequestParams p;
	p.request_info.method = method;
	p.request_info.url = std::move(url);
    p.request_info.body = std::make_shared<net::HttpRequestBufferBody>(
        "application/json;charset=UTF-8", params.toJSON());

    ParseHeader(p, std::move(headers));

    co::Scheduler* s = WorkerScheduler().Get();
	s->go([&, p, params, options](){
      auto res = impl_->Get(p);

      JSValue o = JS_NewObject(params.ctx);
      qjs::Value ret {params.ctx, JS_DupValue(params.ctx, o)};
      GenerateResponse(ret, res);

	  RenderScheduler::Get()->go([ctx = options.ctx, options, ret](){
        JSValue axios_cb = JS_GetPropertyStr(ctx, options.v, "axios_cb");
        JSValue user_cb = JS_GetPropertyStr(ctx, options.v, "user_cb");
        JSValue _this = JS_GetPropertyStr(ctx, options.v, "_this");

		JS_SetPropertyStr(ctx, ret.v, "_u_cb", user_cb);

		JSValue args[] = { ret.v };
		JS_Call(ctx, axios_cb, _this, 1, args);
	  });
	});
}

void NetHttp::Get(String url, qjs::Value params, qjs::Value headers, const qjs::Value& options) {
	Request(std::move(url), net::Method::GET, std::move(params), std::move(headers), options);
}

void NetHttp::Post(String url, qjs::Value params, qjs::Value headers, const qjs::Value& options)
{
    Request(std::move(url), net::Method::POST, std::move(params), std::move(headers), options);
}

void NetHttp::ParseHeader(net::RequestParams& p, qjs::Value headers) {
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
}

void NetHttp::GenerateResponse(qjs::Value& ret, net::HttpResponseInfo* res) {
    ret["code"] = res->status.code;
    ret["msg"] = res->status.description;
    ret["header"] = res->headers; // qjs::js_traits<net::HttpHeaders>::wrap(params.ctx, res->headers);

    std::string_view buf;
    const int bytes = 1024;
    res->body->Read(&buf, bytes);
    char buffer[bytes];
    memcpy(buffer, buf.data(), bytes);

    ret["data"] = String(buffer);
}

}
}