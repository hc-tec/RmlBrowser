//
// Created by titto on 2023/2/5.
//

#ifndef RMLUI_CORE_NETSTREAMFILE_H
#define RMLUI_CORE_NETSTREAMFILE_H

#include "StreamFile.h"

namespace Rml {

class IOBuffer {
public:

    IOBuffer();

    virtual ~IOBuffer();

    virtual void Reset();

    virtual void SetPosition(int pos);

    virtual void Forward(int offset);

    virtual size_t ReadRemainAll(std::string_view* buf);

    virtual size_t Read(std::string_view* buf, size_t buf_size);
    virtual size_t Buffer(char* buf, size_t buf_size);
    virtual size_t GetSize();

protected:
    size_t pos_;
    std::string buffer_;
};

class NetStreamFile : public StreamFile {
public:
	NetStreamFile();
	~NetStreamFile() override;
    /// Attempts to open the stream pointing at a given location.
    bool Open(const String& path) override;
	void Close() override;
	size_t Length() const override;
	size_t Tell() const override;
	bool Seek(long offset, int origin) const override;
	size_t Read(void* buffer, size_t bytes) const override;

private:
	IOBuffer buffer_;
};

}

#endif // RMLUI_CORE_NETSTREAMFILE_H
