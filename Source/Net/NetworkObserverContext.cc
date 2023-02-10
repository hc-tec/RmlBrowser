//
// Created by titto on 2023/2/10.
//

#include "NetworkObserverContext.h"

#include <utility>

namespace Rml {
namespace Net {

void NetworkObserverContext::AddFineGrainRequestObserver(std::weak_ptr<net::HttpRequestObserver> observer) {
	global_fine_request_observers_.Push(std::move(observer));
}

void NetworkObserverContext::ApplyFineGrainRequestObserverToLoader(net::URLLoader* loader) {
    auto observers = global_fine_request_observers_.GetElements();
    for(auto& [_, observer] : observers) {
        if (global_fine_request_observers_.StillAlive(observer))
        {
			auto w_obr = std::any_cast<std::weak_ptr<net::HttpRequestObserver>>(observer);
            loader->AddHttpRequestObserver(w_obr);
        }
    }
    global_fine_request_observers_.ClearUnlessElements();
}


}
}