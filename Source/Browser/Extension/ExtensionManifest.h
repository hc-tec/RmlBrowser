//
// Created by titto on 2023/5/6.
//

#ifndef RMLUI_EXTENSIONMANIFEST_H
#define RMLUI_EXTENSIONMANIFEST_H

#include <json/json.h>

#include "RmlUi/Core/Types.h"


namespace Rml {
namespace Browser {

class ExtensionManifest {
public:
	ExtensionManifest(const String& base_dir, String extension_name);
	const Json::Value& json() { return json_; }
	const String& path() { return path_; }

    const String& name() { return extension_name_; }

private:
	void Parse(const String& mainfest);

	String extension_name_;
	String path_;
    Json::Value json_;
};

}
}
#endif // RMLUI_EXTENSIONMANIFEST_H
