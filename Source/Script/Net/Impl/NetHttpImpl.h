//
// Created by titto on 2023/4/20.
//

#ifndef RMLUI_NETHTTPIMPL_H
#define RMLUI_NETHTTPIMPL_H

#include "RmlUi/Core/Types.h"
#include "Net/Network.h"
#include "core/http/request/http_request_body.h"
#include "core/http/response/http_response_info.h"

namespace Rml {

using namespace Net;

namespace Script {

class NetHttpImpl : public net::HttpRequestObserver {
public:

    NetHttpImpl();

    net::HttpResponseInfo* Get(net::RequestParams params);

	/* HttpRequestObserver */
	void OnResponseAllReceived(
		net::HttpNetworkSession* session,
		net::HttpRequestInfo* request_info,
		net::HttpResponseInfo* response_info) override;


    void SetBaseUrl(String base_url);
    String base_url() { return base_url_; }

    std::shared_ptr<net::HttpResponseInfo> response() { return response_; }

private:
	Network* net_;
    String base_url_;

    std::shared_ptr<net::HttpResponseInfo> response_;
};

}
}

#endif // RMLUI_NETHTTPIMPL_H
