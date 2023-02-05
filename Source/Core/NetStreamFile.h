//
// Created by titto on 2023/2/5.
//

#ifndef RMLUI_CORE_NETSTREAMFILE_H
#define RMLUI_CORE_NETSTREAMFILE_H

#include "StreamFile.h"

namespace Rml {

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
};

}

#endif // RMLUI_CORE_NETSTREAMFILE_H
