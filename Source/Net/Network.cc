//
// Created by titto on 2023/2/9.
//

#include "Network.h"

#include <utility>

namespace Rml {
namespace Net {

Network::Network()
    : service_(net::GetNetworkService()),
		observer_context_(std::make_unique<NetworkObserverContext>()) {}

std::unique_ptr<net::URLLoader> Network::CreateURLLoader(net::RequestParams& request_params) {
    net::HttpRequestHeaders& headers = request_params.request_info.headers;
	if (headers.GetHeader(net::HttpHeaders::ACCEPT_ENCODING).empty()) headers.PutHeaders(net::HttpHeaders::ACCEPT_ENCODING, "");
    if (headers.GetHeader(net::HttpHeaders::ACCEPT).empty()) headers.PutHeaders(net::HttpHeaders::ACCEPT, "*/*");
    if (headers.GetHeader(net::HttpHeaders::HOST).empty()) headers.PutHeaders(net::HttpHeaders::HOST, request_params.request_info.url.host());
    if (headers.GetHeader(net::HttpHeaders::USER_AGENT).empty()) headers.PutHeaders(net::HttpHeaders::USER_AGENT, "RmlBrowser/0.0.1");
    if (headers.GetHeader(net::HttpHeaders::CONNECTION).empty()) headers.PutHeaders(net::HttpHeaders::CONNECTION, net::HttpHeaders::Value::CONNECTION_CLOSE);

    std::unique_ptr<net::URLLoader> loader = service_->CreateURLLoader(request_params);
    observer_context_->ApplyFineGrainRequestObserverToLoader(loader.get());
    return loader;
}

void Network::AddFineGrainRequestObserver(std::weak_ptr<net::HttpRequestObserver> observer) {
    observer_context_->AddFineGrainRequestObserver(std::move(observer));
}

void Network::AddCoarseGrainRequestObserver(std::weak_ptr<net::URLRequestObserver> observer) {
    service_->AddURLRequestObserver(std::move(observer));
}

void Network::AddURLLoaderInterceptor(std::weak_ptr<net::URLLoaderInterceptor> interceptor) {
	service_->AddURLLoaderInterceptor(std::move(interceptor));
}

void Network::RemoveURLLoaderInterceptor(std::weak_ptr<net::URLLoaderInterceptor> interceptor) {
	service_->RemoveURLLoaderInterceptor(std::move(interceptor));
}

}
}