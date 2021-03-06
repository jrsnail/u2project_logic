﻿//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2ZipStream.h"

#include "U2MemoryAllocatorConfig.h"
#include "U2Exception.h"




U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ZipInStream::ZipInStream(const String& name, ZZIP_FILE* zzipFile, size_t uncompressedSize)
: FilterInStream(GET_OBJECT_TYPE(ZipInStream), name)
, mZzipFile(zzipFile)
{
	m_uSize = uncompressedSize;
}
//-----------------------------------------------------------------------
ZipInStream::ZipInStream(const String& name, va_list argp)
    : FilterInStream(GET_OBJECT_TYPE(ZipInStream), name)
{
}
//-----------------------------------------------------------------------
ZipInStream::~ZipInStream()
{
	close();
}
//-----------------------------------------------------------------------
size_t ZipInStream::read(u2byte* s, size_t n)
{
	size_t was_avail = mCache.read(s, n);
	zzip_ssize_t r = 0;
	if (was_avail < n)
	{
		r = zzip_file_read(mZzipFile, (char*)s + was_avail, n - was_avail);
		if (r < 0) {
			ZZIP_DIR *dir = zzip_dirhandle(mZzipFile);
			String msg = zzip_strerror_of(dir);
			U2_EXCEPT(Exception::ERR_INTERNAL_ERROR,
				m_szName + " - error from zziplib: " + msg,
				"ZipInStream::read");
		}
		mCache.cacheData((char*)s + was_avail, (size_t)r);
	}
	return was_avail + (size_t)r;
}
//-----------------------------------------------------------------------
u2sszie_t ZipInStream::skip(u2sszie_t count)
{
	size_t uStart = tell();

	long was_avail = static_cast<long>(mCache.avail());
	if (count > 0)
	{
		if (!mCache.ff(count))
			zzip_seek(mZzipFile, static_cast<zzip_off_t>(count - was_avail), SEEK_CUR);
	}
	else if (count < 0)
	{
		if (!mCache.rewind((size_t)(-count)))
			zzip_seek(mZzipFile, static_cast<zzip_off_t>(count + was_avail), SEEK_CUR);
	}

	return tell() - uStart;
}
//-----------------------------------------------------------------------
void ZipInStream::seek(size_t pos)
{
	zzip_off_t newPos = static_cast<zzip_off_t>(pos);
	zzip_off_t prevPos = static_cast<zzip_off_t>(tell());
	if (prevPos < 0)
	{
		// seek set after invalid pos
		mCache.clear();
		zzip_seek(mZzipFile, newPos, SEEK_SET);
	}
	else
	{
		// everything is going all right, relative seek
		skip((long)(newPos - prevPos));
	}
}
//-----------------------------------------------------------------------
size_t ZipInStream::tell(void) const
{
	zzip_off_t pos = zzip_tell(mZzipFile);
	if (pos < 0)
		return (size_t)(-1);
	return static_cast<size_t>(pos) - mCache.avail();
}
//-----------------------------------------------------------------------
bool ZipInStream::eof() const
{
	return (tell() >= m_uSize);
}
//-----------------------------------------------------------------------
void ZipInStream::close()
{
	if (mZzipFile != 0)
	{
		zzip_file_close(mZzipFile);
		mZzipFile = 0;
	}
	mCache.clear();
}
/*
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ZipOutStream::ZipOutStream(const String& name, FILE* handle)
    : OutStream(GET_OBJECT_TYPE(ZipOutStream), name)
{
}
//-----------------------------------------------------------------------
ZipOutStream::ZipOutStream(const String& name, va_list argp)
    : OutStream(GET_OBJECT_TYPE(ZipInStream), name)
{
}
//-----------------------------------------------------------------------
ZipOutStream::~ZipOutStream()
{
}
//-----------------------------------------------------------------------
size_t ZipOutStream::write(const u2byte* s, size_t n)
{
	assert(0);
	// not supported
	return 0;
}
//-----------------------------------------------------------------------
void ZipOutStream::close()
{
	if (mZzipFile != 0)
	{
		zzip_file_close(mZzipFile);
		mZzipFile = 0;
	}
	mCache.clear();
}
*/