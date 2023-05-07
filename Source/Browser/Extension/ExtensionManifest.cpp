//
// Created by titto on 2023/5/6.
//

#include "ExtensionManifest.h"

#include <fstream>

#include "../WorkerScheduler.h"

namespace Rml {
namespace Browser {

ExtensionManifest::ExtensionManifest(const String& base_dir, String extension_name)
: extension_name_(std::move(extension_name)) {
    path_ = base_dir + extension_name_ + "/";
    String mainfest = path_ + "manifest.json";
    Parse(mainfest);
}

void ExtensionManifest::Parse(const String& mainfest) {
	co::Event event;
	WorkerScheduler().Get()->go([this, event, mainfest](){
      std::ifstream ifs(mainfest);
      Json::Reader reader;
      reader.parse(ifs, json_);
	  event.signal();
	});
    event.wait();
}

}
}