//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2FileHandleStream.h"

#include "U2MemoryAllocatorConfig.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
FileHandleInStream::FileHandleInStream(const String& name, FILE* handle)
: InStream(GET_OBJECT_TYPE(FileHandleInStream), name)
, mFileHandle(handle)
{
}
//-----------------------------------------------------------------------
FileHandleInStream::FileHandleInStream(const String& name, va_list argp)
    : InStream(GET_OBJECT_TYPE(FileHandleInStream), name)
    , mFileHandle(nullptr)
{
}
//-----------------------------------------------------------------------
FileHandleInStream::~FileHandleInStream()
{
	close();
}
//-----------------------------------------------------------------------
size_t FileHandleInStream::read(u2byte* s, size_t n)
{
	return fread(s, 1, n, mFileHandle);
}
//-----------------------------------------------------------------------
ssize_t FileHandleInStream::skip(ssize_t count)
{
	size_t uOrigin = tell();
	fseek(mFileHandle, count, SEEK_CUR);
	return tell() - uOrigin;
}
//-----------------------------------------------------------------------
size_t FileHandleInStream::tell(void) const
{
	return ftell(mFileHandle);
}
//-----------------------------------------------------------------------
bool FileHandleInStream::eof() const
{
	return feof(mFileHandle) != 0;
}
//-----------------------------------------------------------------------
void FileHandleInStream::close()
{
	if (mFileHandle != nullptr)
	{
		fclose(mFileHandle);
		mFileHandle = nullptr;
	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
FileHandleOutStream::FileHandleOutStream(const String& name, FILE* handle)
    : OutStream(GET_OBJECT_TYPE(FileHandleOutStream), name)
    , mFileHandle(nullptr)
{
}
//-----------------------------------------------------------------------
FileHandleOutStream::FileHandleOutStream(const String& name, va_list argp)
    : OutStream(GET_OBJECT_TYPE(FileHandleInStream), name)
    , mFileHandle(nullptr)
{
}
//-----------------------------------------------------------------------
FileHandleOutStream::~FileHandleOutStream()
{
}
//-----------------------------------------------------------------------
size_t FileHandleOutStream::write(const u2byte* s, size_t n)
{
	return fwrite(s, 1, n, mFileHandle);
}
//-----------------------------------------------------------------------
void FileHandleOutStream::close()
{
	if (mFileHandle != nullptr)
	{
		fclose(mFileHandle);
		mFileHandle = nullptr;
	}
}