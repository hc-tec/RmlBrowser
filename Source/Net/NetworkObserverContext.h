//
// Created by titto on 2023/2/10.
//

#ifndef RMLUI_NETWORKOBSERVERCONTEXT_H
#define RMLUI_NETWORKOBSERVERCONTEXT_H

#include "core/base/weak_container.h"
#include "core/url_loader/url_loader.h"
#include "core/http/http_request_observer.h"

using namespace tit;

namespace Rml {
namespace Net {

class NetworkObserverContext {
public:

    void AddFineGrainRequestObserver(std::weak_ptr<net::HttpRequestObserver> observer);

	void ApplyFineGrainRequestObserverToLoader(net::URLLoader* loader);

private:
    // observe all requests in fine-grain
    WeakContainerUnsafe<net::HttpRequestObserver> global_fine_request_observers_;
};

}
}
#endif // RMLUI_NETWORKOBSERVERCONTEXT_H
