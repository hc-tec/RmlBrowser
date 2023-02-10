//
// Created by titto on 2023/2/5.
//

#include "NetStreamFile.h"

#include <cmath>

#include "Net/Network.h"
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
    Net::Network* service = Net::Network::GetInstance();

    net::RequestParams params;
    params.request_info.url = net::URL(path);
    params.request_info.method = net::Method::GET;
    std::unique_ptr<net::URLLoader> loader = service->CreateURLLoader(params);
    loader->AddHttpRequestObserver(this);
    loader->Start();
    loader->RemoveHttpRequestObserver(this);
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

size_t NetStreamFile::GetSize()
{
	return buffer_->GetSize();
}

}
