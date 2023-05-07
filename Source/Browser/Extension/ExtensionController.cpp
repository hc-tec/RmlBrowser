//
// Created by titto on 2023/5/5.
//

#include "ExtensionController.h"

#include "../RmlContext.h"
#include "ExtensionManifest.h"
#include "core/http/response/http_response_info.h"
namespace Rml {

namespace Browser {

ExtensionController::ExtensionController(UniquePtr<ExtensionManifest> manifest)
    : context_(RmlContext::Get()),
      script_plugin_(MakeUnique<Script::ScriptPlugin>()),
		cur_tab_(nullptr),
		document_(nullptr),
	    manifest_(std::move(manifest)),
      close_cause_unactive_(false),
      is_active_(false),
      unactive_time_(0) {
	String popup = manifest_->json()["action"]["default_popup"].asString();
    document_ = context_->LoadDocument(manifest_->path() + popup);
	if (document_ == nullptr) return;
	Glue();
    document_->Hide();
	document_->SetProperty("z-index", "999");
	script_plugin_->OnDocumentLoad(document_);
}

ExtensionController::~ExtensionController() {
	script_plugin_.reset();
	manifest_.reset();
    Net::Network::GetInstance()->RemoveCoarseGrainRequestObserver(this);
}

void ExtensionController::Glue() {
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value rml = js_context->newObject();
    rml["extension"] = js_context->newObject();
    rml["dom"] = js_context->newObject();
    rml["tab"] = js_context->newObject();
    rml["net"] = js_context->newObject();
    rml["net"]["observeNetwork"] = [=](){
		Net::Network::GetInstance()->AddCoarseGrainRequestObserver(this);
	};
    rml["net"]["cancelObserveNetwork"] = [=](){
        Net::Network::GetInstance()->RemoveCoarseGrainRequestObserver(this);
    };
    js_context->global()["rml"] = rml;
}

void ExtensionController::OnInitialise() {
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["extension"]["onInitialise"];
    if (JS_IsUndefined(cb.v)) return;
    static_cast<std::function<void()>>(cb)();
}

void ExtensionController::OnShutdown() {
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["extension"]["onShutdown"];
    if (JS_IsUndefined(cb.v)) return;
    static_cast<std::function<void()>>(cb)();
}

void ExtensionController::OnContextCreate(Context* context) {  }
void ExtensionController::OnContextDestroy(Context* context) {  }
void ExtensionController::OnDocumentOpen(Context* context, const String& document_path) {  }

void ExtensionController::OnDocumentLoad(ElementDocument* document) {
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["dom"]["onDocumentLoad"];
    if (JS_IsUndefined(cb.v)) return;
    static_cast<std::function<void(ElementDocument*)>>(cb)(document);
}

void ExtensionController::OnDocumentUnload(ElementDocument* document) {
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["dom"]["onDocumentUnload"];
    if (JS_IsUndefined(cb.v)) return;
    static_cast<std::function<void(ElementDocument*)>>(cb)(document);
}

void ExtensionController::OnElementCreate(Element* element) {
	if (Filter(element)) return;
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["dom"]["onElementCreate"];
    if (JS_IsUndefined(cb.v)) return;
    static_cast<std::function<void(Element*)>>(cb)(element);
}

void ExtensionController::OnElementDestroy(Element* element) {
    if (Filter(element)) return;
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["dom"]["onElementDestroy"];
    if (JS_IsUndefined(cb.v)) return;
    static_cast<std::function<void(Element*)>>(cb)(element);
}

void ExtensionController::OnTabStopRunning(Tab* tab) {
    if (Filter(tab)) return;
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["tab"]["onStopRunning"];
    if (JS_IsUndefined(cb.v)) return;
    static_cast<std::function<void(Tab*)>>(cb)(tab);
}

void ExtensionController::OnTabActive(Tab* tab) {
    if (Filter(tab)) {
		if (!cur_tab_) return;
		if (unactive_time_ == 0)
		{
			is_active_ = document_->IsVisible();
		}
        unactive_time_++;
		close_cause_unactive_ = true;
        document_->Hide();
		return;
	}
    unactive_time_ = 0;
	if (close_cause_unactive_ && cur_tab_) {
		close_cause_unactive_ = false;
		if (is_active_) document_->Show();
	}
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["tab"]["onActive"];
	if (JS_IsUndefined(cb.v)) return;
	static_cast<std::function<void(Tab*)>>(cb)(tab);
}

void ExtensionController::OnRequestStart(net::URLRequest* request, net::HttpRequestInfo* requestInfo)
{
	String tab_id = requestInfo->headers.GetHeader("browser-tab-id");
	if (Filter(tab_id)) return;
    qjs::Context* js_context = script_plugin_->js_context();
    RenderScheduler::Get()->go([js_context,
								   url = requestInfo->url.origin_url(),
								   method = MethodToString(requestInfo->method),
								   body = requestInfo->body ? requestInfo->body->ToString() : "",
								   headers = requestInfo->headers](){

        qjs::Value cb = js_context->global()["rml"]["net"]["onRequestStart"];
        if (JS_IsUndefined(cb.v)) return;
        qjs::Value obj = js_context->newObject();
        obj["url"] = url;
        obj["method"] = method;
        obj["body"] = body;
        obj["header"] = headers;
        static_cast<std::function<void(qjs::Value)>>(cb)(obj);
	});
}

void ExtensionController::OnRequestEnd(net::URLRequest* request, net::HttpRequestInfo* requestInfo, net::HttpResponseInfo* responseInfo)
{
    String tab_id = requestInfo->headers.GetHeader("browser-tab-id");
    if (Filter(tab_id)) return;
    qjs::Context* js_context = script_plugin_->js_context();
    RenderScheduler::Get()->go([js_context,
                                   url = requestInfo->url.origin_url(),
                                   method = MethodToString(requestInfo->method),
                                   body = requestInfo->body ? requestInfo->body->ToString() : "",
								   headers = requestInfo->headers,
								   res_h = responseInfo->headers,
								   res_b = responseInfo->body->ToString(),
								   res_s = responseInfo->status](){

      qjs::Value cb = js_context->global()["rml"]["net"]["onRequestEnd"];
      if (JS_IsUndefined(cb.v)) return;
      qjs::Value ret = js_context->newObject();

      qjs::Value obj = js_context->newObject();
      obj["url"] = url;
      obj["method"] = method;
      obj["body"] = body;
      obj["header"] = headers;

      ret["request"] = obj;

      obj = js_context->newObject();
	  obj["header"] = res_h;
      obj["body"] = res_b;
      obj["status"] = res_s.code;

      ret["response"] = obj;
      static_cast<std::function<void(qjs::Value)>>(cb)(ret);
    });
}

void ExtensionController::OnRequestError(net::URLRequest* request, int err_code) {
    String tab_id = request->request_info()->headers.GetHeader("browser-tab-id");
    if (Filter(tab_id)) return;
}

void ExtensionController::OnExtensionIconClick(Tab* tab, const String& name, qjs::Value event) {
	if (Filter(tab)) return;
    qjs::Context* js_context = script_plugin_->js_context();
    qjs::Value cb = js_context->global()["rml"]["extension"]["onClick"];
    if (JS_IsUndefined(cb.v)) return;
    static_cast<std::function<void(qjs::Value)>>(cb)(std::move(event));
}

bool ExtensionController::Filter(Element* element) { return cur_tab_ && cur_tab_->document() != element->GetOwnerDocument(); }

bool ExtensionController::Filter(Tab* tab) { return cur_tab_ && cur_tab_ != tab; }

bool ExtensionController::Filter(const String& tab_id) { return cur_tab_ && cur_tab_->tab_id() != tab_id; }

}

}