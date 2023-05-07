//
// Created by titto on 2023/4/26.
//

#ifndef RMLUI_RESOURCELOADER_H
#define RMLUI_RESOURCELOADER_H

#include "NetStreamFile.h"
#include <co/co.h>
#include <utility>
#include <unordered_map>
namespace Rml {

enum class LoadStatus {
	kNotLoad = 0,
    kLoading,
    kFinish,
    kFail,
};

using Resource = struct Resource {
	String url;
	NetStreamFile file;
    LoadStatus status;
	bool need_clear_after_finish;
	co::Event* event;

	Resource(String url_, LoadStatus status_, bool need_clear_after_finish_)
	    : url(std::move(url_)),
		status(status_),
		need_clear_after_finish(need_clear_after_finish_),
	    event(nullptr) {}
};

class ResourceLoader {
public:
    static ResourceLoader* Get() {
		static ResourceLoader loader;
		return &loader;
	}

    void Load(String url, bool need_clear_after_finish = true);

    LoadStatus WaitForResource(const String& url, NetStreamFile* file);

    LoadStatus TryGetResource(const String& url, NetStreamFile* file);

	void Clear();

private:

	void NextScheduler();

    std::unordered_map<String, UniquePtr<Resource>> resource_map_;
    co::Mutex mutex_;
	co::Scheduler* s_{};
};

}

#endif // RMLUI_RESOURCELOADER_H
