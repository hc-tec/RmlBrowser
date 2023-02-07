//
// Created by titto on 2023/2/5.
//

#include "NetStreamFile.h"

#include <cmath>

#include "core/http/request/http_request_body.h"
#include "core/network/network_service.h"
#include "core/network/request_params.h"
#include "core/socket/tcp/address.h"

using namespace tit;

namespace Rml {

IOBuffer::IOBuffer()
    : pos_(0),
      buffer_() {}

size_t IOBuffer::GetSize() {
    return buffer_.size();
}

size_t IOBuffer::Buffer(char *buf, size_t buf_size) {
    buffer_.append(buf, buf_size);
    return buf_size;
}

void IOBuffer::Reset() {
    pos_ = 0;
}

void IOBuffer::SetPosition(int pos) {
    pos_ = pos;
}

size_t IOBuffer::Read(std::string_view *buf, size_t buf_size) {
    if (pos_ > GetSize()) {
		Log::Message(Log::LT_ERROR, "buffer out of size");
        return -1;
    }
    int remain = GetSize() - pos_;
    remain = fmin(remain, buf_size);
    *buf = std::string_view(buffer_.data()+pos_, remain);
//  memcpy(buf, buffer_.data()+pos_, remain);
    Forward(remain);
    return remain;
}

size_t IOBuffer::ReadRemainAll(std::string_view *buf) {
    return Read(buf, GetSize() - pos_);
}

void IOBuffer::Forward(int offset) {
    pos_ += offset;
}

IOBuffer::~IOBuffer() {}

NetStreamFile::NetStreamFile() {}

NetStreamFile::~NetStreamFile() {}

void NetStreamFile::Close()
{
	StreamFile::Close();
}

size_t NetStreamFile::Length() const
{
	return StreamFile::Length();
}

size_t NetStreamFile::Tell() const
{
	return StreamFile::Tell();
}

bool NetStreamFile::Seek(long offset, int origin) const
{
	return StreamFile::Seek(offset, origin);
}

size_t NetStreamFile::Read(void* buffer, size_t bytes) const
{
	return StreamFile::Read(buffer, bytes);
}

bool NetStreamFile::Open(const String& path)
{
    net::NetworkService* service = net::GetNetworkService();
    net::RequestParams params;
    params.request_info.url = net::URL("https://www.zhihu.com/sign");
    params.request_info.method = net::Method::GET;
    net::HttpRequestHeaders& headers = params.request_info.headers;

    headers.PutHeaders("accept-encoding", "");
    headers.PutHeaders("accept", "*/*");
//    headers.PutHeaders(net::HttpHeaders::CONNECTION,
//                       net::HttpHeaders::Value::CONNECTION_KEEP_ALIVE);
    headers.PutHeaders("host", params.request_info.url.host());
    headers.PutHeaders("user-agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/101.0.4951.64 Safari/537.36 Edg/101.0.1210.53");
    std::unique_ptr<net::URLLoader> loader = service->CreateURLLoader(params);
    loader->Start();
	return false;
}

}
