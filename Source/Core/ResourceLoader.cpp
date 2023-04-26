//
// Created by titto on 2023/4/26.
//

#include "ResourceLoader.h"



namespace Rml {

void ResourceLoader::Load(String url, bool need_clear_after_finish) {
    URL u(url);
	if (u.GetProtocol() == "file") return;
    {
        co::MutexGuard g(mutex_);
        resource_map_[url] = MakeUnique<Resource>(url, LoadStatus::kLoading, need_clear_after_finish);
    }

    NextScheduler();
	s_->go([=](){
        Resource* resource = resource_map_[url].get();
		if (resource == nullptr) return;
		if (resource->file.Open(url))
		{
			resource->status = LoadStatus::kFinish;
            if (resource->event != nullptr) {
				resource->event->signal();
			}
		} else {
            resource->status = LoadStatus::kFail;
		}
	});
}

LoadStatus ResourceLoader::TryGetResource(const String& url, NetStreamFile* file) {
    co::MutexGuard g(mutex_);
	auto it = resource_map_.find(url);
	if (it == resource_map_.end()) {
		return LoadStatus::kNotLoad;
	}
	if (resource_map_[url]->file.is_open())
	{
		*file = resource_map_[url]->file;
		return LoadStatus::kFinish;
	}
	return LoadStatus::kLoading;
}

LoadStatus ResourceLoader::WaitForResource(const String& url, NetStreamFile* file) {
	NetStreamFile f;
    LoadStatus status = TryGetResource(url, &f);
	if (status == LoadStatus::kFinish) {
		*file = f;
		return status;
	} else if (status == LoadStatus::kNotLoad) {
        return status;
	}
	co::Event event;
    {
        co::MutexGuard g(mutex_);
		resource_map_[url]->event = &event;
	}
	event.wait();
	return LoadStatus::kFinish;
}

void ResourceLoader::NextScheduler() {
	s_ = co::next_scheduler();
	if (s_ == co::schedulers()[0]) NextScheduler();
}

}