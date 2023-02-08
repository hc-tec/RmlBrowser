//
// Created by titto on 2023/2/5.
//

#include "NetStreamFile.h"

#include <cmath>

#include "core/http/request/http_request_body.h"
#include "core/http/response/http_response_info.h"
#include "core/network/network_service.h"
#include "core/network/request_params.h"


using namespace tit;

namespace Rml {

NetStreamFile::NetStreamFile() {}

NetStreamFile::~NetStreamFile() {}

void NetStreamFile::Close()
{

}

size_t NetStreamFile::Length() const
{
	return buffer_->GetSize();
}

size_t NetStreamFile::Tell() const
{
	return buffer_->GetPosition();
}

bool NetStreamFile::Seek(long offset, int origin) const
{
    buffer_->SetPosition(offset);
	return true;
}

size_t NetStreamFile::Read(void* buffer, size_t bytes) const
{
	std::string_view buf;
	int read_size = buffer_->Read(&buf, bytes);
	memcpy(buffer, buf.data(), bytes);
	return read_size;
}

bool NetStreamFile::Open(const String& path)
{
    SetStreamDetails(URL(path), Stream::MODE_READ);

    net::NetworkService* service = net::GetNetworkService();

    net::RequestParams params;
    params.request_info.url = net::URL(path);
    params.request_info.method = net::Method::GET;
    net::HttpRequestHeaders& headers = params.request_info.headers;
    headers.PutHeaders("accept-encoding", "");
    headers.PutHeaders("accept", "*/*");
//    headers.PutHeaders(net::HttpHeaders::CONNECTION,
//                       net::HttpHeaders::Value::CONNECTION_KEEP_ALIVE);
    headers.PutHeaders("host", params.request_info.url.host());
    headers.PutHeaders("user-agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.64 Safari/537.36 Edg/101.0.1210.53");
    std::unique_ptr<net::URLLoader> loader = service->CreateURLLoader(params);
    loader->AddHttpRequestObserver(shared_from_this());
    loader->Start();
	return true;
}

void NetStreamFile::OnResponseAllReceived(
	net::HttpNetworkSession* session,
	net::HttpRequestInfo* request_info,
	net::HttpResponseInfo* response_info)
{
	buffer_ = std::dynamic_pointer_cast<net::HttpResponseBufferBody>(
		response_info->body);
}

}
