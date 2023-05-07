//
// Created by titto on 2023/5/6.
//

#ifndef RMLUI_EXTENSION_MANAGER_H
#define RMLUI_EXTENSION_MANAGER_H

#include <json/json.h>

#include "RmlUi/Core/Types.h"
#include "ExtensionController.h"

namespace Rml {
namespace Browser {


class ExtensionManager {
public:
	class Delegate {
	public:
		virtual void OnExtensionLoad(const Vector<Json::Value>& extension_info) = 0;
	};
public:
	ExtensionManager(Delegate* delegate);

	void LoadSingletonExtensions();
    void LoadEveryTabExtensions(Tab* tab);
    void DoExtensionClick(Tab* tab, const String& name, const qjs::Value& event);
    const Vector<Json::Value>& extension_info() { return extension_info_; }

private:
    Delegate* delegate_;

	UnorderedMap<String, Vector<String>> installed_extensions_;
	Vector<UniquePtr<ExtensionController>> extension_controllers_;
	Vector<Json::Value> extension_info_;
};

}
}
#endif // RMLUI_EXTENSION_MANAGER_H
