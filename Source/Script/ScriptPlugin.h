//
// Created by titto on 2022/12/30.
//

#ifndef RMLUI_SCRIPTPLUGIN_H
#define RMLUI_SCRIPTPLUGIN_H


#include "core/http/http_request_observer.h"

#include "RmlUi/Core/Plugin.h"
#include "Dom/Document.h"

using namespace tit;

namespace Rml {

namespace Script {

class Document;
class JsDocumentElementInstancer;

class ScriptPlugin : public Rml::Plugin {
public:
	class Delegate {
	public:
		virtual void OnDocumentLoad(ElementDocument* document) = 0;
	};
public:

	ScriptPlugin(Context* context, Delegate* delegate);

    int GetEventClasses() override;
    void OnInitialise() override;
    void OnShutdown() override;
    void OnContextCreate(Context* context) override;
    void OnContextDestroy(Context* context) override;
    void OnDocumentOpen(Context* context, const String& document_path) override;
    void OnDocumentLoad(ElementDocument* document) override;
    void OnDocumentUnload(ElementDocument* document) override;
    void OnElementCreate(Element* element) override;
    void OnElementDestroy(Element* element) override;
	~ScriptPlugin() override;

	qjs::Context* js_context() { return js_context_.get(); }

	void LoadExternJs(qjs::Context* js_context, const std::string& path);

private:
	Delegate* delegate_;
	Context* context_;
	qjs::Runtime* js_runtime_;
    UniquePtr<qjs::Context> js_context_;
    SharedPtr<JsDocumentElementInstancer> js_document_element_instancer_;
};


}

}



#endif // RMLUI_SCRIPTPLUGIN_H
