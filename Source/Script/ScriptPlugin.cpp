//
// Created by titto on 2022/12/30.
//

#include "ScriptPlugin.h"

#include <iostream>

#include "core/http/response/http_response_info.h"

#include "RmlUi/Core/Context.h"
#include "RmlUi/Core/Element.h"
#include "RmlUi/Core/Factory.h"
#include "RmlUi/Core/ElementDocument.h"
#include "Net/Network.h"

#include <co/co/mutex.h>
#include "Glue.h"
#include "RunTime.h"
#include "Dom/JsDocumentElementInstancer.h"
#include "Dom/XMLNodeHandlerAnchor.h"

namespace Rml {

namespace Script {

ScriptPlugin::ScriptPlugin(Context* context)
		: context_(context),
		js_runtime_(nullptr) {
//    static co::mutex js_mutex;
//    auto _ = co::mutex_guard(js_mutex);
    js_runtime_ = GetRunTime(); // new qjs::Runtime();
    js_context_ = MakeUnique<qjs::Context>(js_runtime_->rt);
    Glue(js_context_.get());
    js_document_element_instancer_ = MakeShared<JsDocumentElementInstancer>();
    Factory::RegisterElementInstancer("body", js_document_element_instancer_.get());
    XMLParser::RegisterNodeHandler("a", MakeShared<XMLNodeHandlerAnchor>());
    XMLParser::RegisterNodeHandler("script", js_document_element_instancer_);
}

int ScriptPlugin::GetEventClasses() { return Plugin::GetEventClasses(); }

void ScriptPlugin::OnInitialise() {
    std::cout << "Register ScriptPlugin" << std::endl;
}

void ScriptPlugin::OnShutdown() { Plugin::OnShutdown(); }

void ScriptPlugin::OnContextCreate(Context* context) {
//    context_ = context;

}

void ScriptPlugin::OnContextDestroy(Context* context) {
    if (context != context_) return;
}

void ScriptPlugin::OnDocumentOpen(Context* context, const String& document_path) {
    if (context != context_) return;
}

void ScriptPlugin::OnDocumentLoad(ElementDocument* document) {
	if (document->GetContext() != context_) return;
    qjs::Context* js_context = js_context_.get();
    js_context->global()["document"] = document;
	DocumentHeader::ResourceList scripts = js_document_element_instancer_->GetScripts();
    try {
//        static co::mutex js_mutex;
//        auto _ = co::mutex_guard(js_mutex);
        js_context->global()["executing"] = true;
        for (auto& script : scripts) {
            if (script.is_inline) {
                js_context->eval(script.content, script.path.data());
            } else {
				LoadExternJs(js_context, script.path);
			}
        }
        js_context->global()["executing"] = false;
    } catch (qjs::exception) {
        auto exc = js_context->getException();
        std::cerr << (std::string) exc << std::endl;
        if((bool) exc["stack"])
            std::cerr << (std::string) exc["stack"] << std::endl;
    }
}

void ScriptPlugin::OnDocumentUnload(ElementDocument* document) {
//	document_.reset();
    if (document->GetContext() != context_) return;
}

void ScriptPlugin::OnElementCreate(Element* element) {
//	element->GetParentNode()
}

void ScriptPlugin::OnElementDestroy(Element* element) {


}

ScriptPlugin::~ScriptPlugin() {
    js_context_.reset();
}

void ScriptPlugin::LoadExternJs(qjs::Context* js_context, const std::string& path) {
    URL url(path);
	if (url.GetProtocol() == "file")
	{
		js_context->evalFile(path.data());
	} else {
        Net::Network* service = Net::Network::GetInstance();
        net::RequestParams params;
        params.request_info.url = net::URL(path);
        params.request_info.method = net::Method::GET;
        std::unique_ptr<net::URLLoader> loader = service->CreateURLLoader(params);
        loader->AddHttpRequestObserver(this);
        loader->Start();
        loader->RemoveHttpRequestObserver(this);
		js_context->eval(js_buffer_.data(), url.GetFileName().data());
	}
}

void ScriptPlugin::OnResponseAllReceived(net::HttpNetworkSession* session, net::HttpRequestInfo* request_info, net::HttpResponseInfo* response_info)
{
	std::string_view buf;
	response_info->body->Read(&buf, response_info->body->GetSize());
	js_buffer_.append(buf.data());
}

}

}