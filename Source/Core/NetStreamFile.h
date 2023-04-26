//
// Created by titto on 2023/2/5.
//

#ifndef RMLUI_CORE_NETSTREAMFILE_H
#define RMLUI_CORE_NETSTREAMFILE_H

#include "StreamFile.h"

#include "core/http/http_request_observer.h"
#include "core/http/response/http_response_body.h"

using namespace tit;

namespace Rml {

class NetStreamFile : public StreamFile,
					  public net::HttpRequestObserver,
					  public std::enable_shared_from_this<NetStreamFile> {
public:
	NetStreamFile();

    NetStreamFile(const NetStreamFile& file)
		: is_open_(file.is_open_),
		buffer_(file.buffer_) {}

    /// Attempts to open the stream pointing at a given location.
    bool Open(const String& path) override;
	void Close() override;
	size_t Length() const override;
	size_t Tell() const override;
	bool Seek(long offset, int origin) const override;
	size_t Read(void* buffer, size_t bytes) const override;

	size_t GetSize();
	// HttpRequestObserver
	void OnResponseAllReceived(net::HttpNetworkSession* session,
		net::HttpRequestInfo* request_info,
		net::HttpResponseInfo* response_info) override;

    bool is_open() const { return is_open_; }

private:
	bool is_open_;
	std::shared_ptr<net::HttpResponseBufferBody> buffer_;
};

}

#endif // RMLUI_CORE_NETSTREAMFILE_H
