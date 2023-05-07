//
// Created by titto on 2023/5/6.
//

#include "ExtensionManager.h"
#include "ExtensionManifest.h"
#include "Browser/BrowserPluginRegistry.h"

namespace Rml {
namespace Browser {

const char* EXTENSION_BASE_DIR = "/home/titto/CProjects/RmlUi5.0/Extensions/";

ExtensionManager::ExtensionManager(Delegate* delegate)
: delegate_(delegate) {
    installed_extensions_["singleton"] = Vector<String>();
    installed_extensions_["every-tab"] = Vector<String>();
	installed_extensions_["every-tab"].emplace_back("DevTools");
}

void ExtensionManager::LoadSingletonExtensions() {
    for (auto& extension : installed_extensions_["singleton"]) {
		UniquePtr<ExtensionManifest> manifest = MakeUnique<ExtensionManifest>(EXTENSION_BASE_DIR, extension);
        extension_info_.push_back(manifest->json());
        UniquePtr<ExtensionController> controller = MakeUnique<ExtensionController>(std::move(manifest));
        BrowserPluginRegistry::RegisterPlugin(controller.get());
		extension_controllers_.push_back(std::move(controller));
	}
}

void ExtensionManager::LoadEveryTabExtensions(Tab* tab) {
	static bool has_notify_loaded = false;
	if (installed_extensions_["every-tab"].empty() && !has_notify_loaded) {
        has_notify_loaded = true;
        if (delegate_) delegate_->OnExtensionLoad(extension_info_);
		return;
	}
    for (auto& extension : installed_extensions_["every-tab"]) {
        UniquePtr<ExtensionManifest> manifest = MakeUnique<ExtensionManifest>(EXTENSION_BASE_DIR, extension);
		if (!has_notify_loaded) extension_info_.push_back(manifest->json());
        UniquePtr<ExtensionController> controller = MakeUnique<ExtensionController>(std::move(manifest));
		controller->set_interested_tab(tab);
        BrowserPluginRegistry::RegisterPlugin(controller.get());
        extension_controllers_.push_back(std::move(controller));
    }
	if (!has_notify_loaded)
	{
        has_notify_loaded = true;
		if (delegate_)
			delegate_->OnExtensionLoad(extension_info_);
	}
}

void ExtensionManager::DoExtensionClick(Tab* tab, const String& name, const qjs::Value& event) {
    for (auto& controller : extension_controllers_) {
		if (controller->manifest()->name() != name) continue;
		controller->OnExtensionIconClick(tab, name, event);
	}
}



}
}