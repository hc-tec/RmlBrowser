//
// Created by titto on 2023/4/20.
//

#include "NetHttpImpl.h"

#include <utility>

#include "Net/Network.h"

namespace Rml {
namespace Script {

NetHttpImpl::NetHttpImpl()
		: net_(Network::GetInstance()) {}

void NetHttpImpl::SetBaseUrl(String base_url) {
	base_url_ = std::move(base_url);
}

net::HttpResponseInfo* NetHttpImpl::Get(net::RequestParams params) {
    std::unique_ptr<net::URLLoader> loader = net_->CreateURLLoader(params);
    loader->AddHttpRequestObserver(this);
    loader->Start();
    loader->RemoveHttpRequestObserver(this);
	return response_.get();
}

void NetHttpImpl::OnResponseAllReceived(net::HttpNetworkSession* session, net::HttpRequestInfo* request_info, net::HttpResponseInfo* response_info)
{
    response_ = MakeShared<net::HttpResponseInfo>(*response_info);
}

}
}