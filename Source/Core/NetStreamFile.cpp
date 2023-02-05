//
// Created by titto on 2023/2/5.
//

#include "NetStreamFile.h"

namespace Rml {

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
	return StreamFile::Open(path);
}

}
