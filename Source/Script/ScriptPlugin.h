//
// Created by titto on 2022/12/30.
//

#ifndef RMLUI_SCRIPTPLUGIN_H
#define RMLUI_SCRIPTPLUGIN_H


#include "RmlUi/Core/Plugin.h"
#include "Dom/Document.h"


namespace Rml {

namespace Script {

class Document;

class ScriptPlugin : public Rml::Plugin {
public:
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

private:

	Context* context_;
};

static ScriptPlugin* plugin = nullptr;
inline ScriptPlugin* GetInstance() {
	if (plugin == nullptr) {
		plugin = new ScriptPlugin();
	}
	return plugin;
}


}

}



#endif // RMLUI_SCRIPTPLUGIN_H
