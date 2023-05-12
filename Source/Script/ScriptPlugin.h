//
// Created by titto on 2022/12/30.
//

#ifndef RMLUI_SCRIPTPLUGIN_H
#define RMLUI_SCRIPTPLUGIN_H


#include "core/http/http_request_observer.h"

#include "RmlUi/Core/Plugin.h"
#include "Dom/Document.h"
#include "Dom/JsDocumentElementInstancer.h"

using namespace tit;

namespace Rml {

namespace Script {

class Document;

class ScriptPlugin : public JsDocumentElementInstancer::Delegate {
public:

	ScriptPlugin();
    ~ScriptPlugin();

    void OnDocumentLoad(ElementDocument* document);
	qjs::Context* js_context() { return js_context_.get(); }

private:
	void ParseCustomComponent();
    void LoadJs();
	void LoadCustomComponentResource(ElementDocument* document);
	void LoadCustomComponentCss(ElementDocument* document);
    void LoadCustomComponentScript();
    void LoadExternJs(qjs::Context* js_context, const std::string& path);

	void OnHeadParseFinish() override;

	ElementDocument* document_;
	Vector<String> builtin_scripts_;
	Vector<String> register_components_;
	qjs::Runtime* js_runtime_;
    UniquePtr<qjs::Context> js_context_;
    SharedPtr<JsDocumentElementInstancer> js_document_element_instancer_;
};

}

}



#endif // RMLUI_SCRIPTPLUGIN_H
