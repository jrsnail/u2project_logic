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
u2int32 FileHandleInStream::read(u2byte* s, std::streamsize n)
{
	return fread(s, 1, n, mFileHandle);
}
//-----------------------------------------------------------------------
std::streamoff FileHandleInStream::skip(std::streamoff count)
{
	fseek(mFileHandle, count, SEEK_CUR);
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
size_t FileHandleOutStream::write(const u2byte* s, std::streamsize n)
{
	return fwrite(s, 1, n, mFileHandle);
}
//-----------------------------------------------------------------------
void FileHandleOutStream::flush()
{
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