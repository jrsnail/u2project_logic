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
FileHandleInStream::FileHandleInStream(const String& name, const char* filename, const char* mode)
	: InStream(GET_OBJECT_TYPE(FileHandleInStream), name)
	, mFileHandle(nullptr)
{
	open(filename, mode);
}
//-----------------------------------------------------------------------
FileHandleInStream::FileHandleInStream(const String& name, va_list argp)
	: InStream(GET_OBJECT_TYPE(FileHandleInStream), name)
	, mFileHandle(nullptr)
{
	const char* pFilename = va_arg(argp, const char*);
	const char* pMode = va_arg(argp, const char*);
	open(pFilename, pMode);
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
u2sszie_t FileHandleInStream::skip(u2sszie_t count)
{
	size_t uOrigin = tell();
	fseek(mFileHandle, count, SEEK_CUR);
	return tell() - uOrigin;
}
//-----------------------------------------------------------------------
void FileHandleInStream::seek(size_t pos)
{
	fseek(mFileHandle, static_cast<long>(pos), SEEK_SET);
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
void FileHandleInStream::open(const char* filename, const char* mode)
{
	assert(mFileHandle == nullptr);
	mFileHandle = fopen(filename, mode);
	if (mFileHandle == nullptr)
	{
		assert(0);
	}
	else
	{
		// Determine size
		fseek(mFileHandle, 0, SEEK_END);
		m_uSize = ftell(mFileHandle);
		fseek(mFileHandle, 0, SEEK_SET);
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
FileHandleOutStream::FileHandleOutStream(const String& name, const char* filename, const char* mode)
	: OutStream(GET_OBJECT_TYPE(FileHandleOutStream), name)
	, mFileHandle(nullptr)
{
	open(filename, mode);
}
//-----------------------------------------------------------------------
FileHandleOutStream::FileHandleOutStream(const String& name, va_list argp)
    : OutStream(GET_OBJECT_TYPE(FileHandleOutStream), name)
    , mFileHandle(nullptr)
{
	const char* pFilename = va_arg(argp, const char*);
	const char* pMode = va_arg(argp, const char*);
	open(pFilename, pMode);
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
//-----------------------------------------------------------------------
void FileHandleOutStream::open(const char* filename, const char* mode)
{
	assert(mFileHandle == nullptr);
	mFileHandle = fopen(filename, mode);
	if (mFileHandle == nullptr)
	{
		assert(0);
	}
}