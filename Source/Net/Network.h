//
// Created by titto on 2023/2/9.
//

#ifndef RMLUI_NETWORK_H
#define RMLUI_NETWORK_H

#include <atomic>

#include "core/base/weak_container.h"
#include "core/network/network_service.h"
#include "core/network/request_params.h"

#include "NetworkObserverContext.h"

using namespace tit;

namespace Rml {
namespace Net {

class Network {
public:
    Network();

	static Network* GetInstance() {
        static Network instance;
		return &instance;
	}

    std::unique_ptr<net::URLLoader> CreateURLLoader(
        net::RequestParams& request_params);

    void AddURLLoaderInterceptor(
        std::weak_ptr<net::URLLoaderInterceptor> interceptor);

    void RemoveURLLoaderInterceptor(
        std::weak_ptr<net::URLLoaderInterceptor> interceptor);

    void AddFineGrainRequestObserver(net::HttpRequestObserver* observer);

    void AddFineGrainRequestObserver(std::weak_ptr<net::HttpRequestObserver> observer);

    void AddCoarseGrainRequestObserver(net::URLRequestObserver* observer);

    void AddCoarseGrainRequestObserver(std::weak_ptr<net::URLRequestObserver> observer);

    void RemoveCoarseGrainRequestObserver(net::URLRequestObserver* observer);

    void RemoveCoarseGrainRequestObserver(std::weak_ptr<net::URLRequestObserver> observer);

private:
    std::atomic<int> net_id_;
    net::NetworkService* service_;
    std::unique_ptr<NetworkObserverContext> observer_context_;
};

}
}

#endif // RMLUI_NETWORK_H
