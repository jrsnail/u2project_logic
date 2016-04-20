//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2ZipStream__
#define __U2ZipStream__


#include "U2Prerequisites.h"
#include "U2FilterStream.h"
#include <zzip/zzip.h>
#include <zzip/plugin.h>
#include "U2STLRedefined.h"


typedef struct zzip_dir			ZZIP_DIR;
typedef struct zzip_file		ZZIP_FILE;
typedef union _zzip_plugin_io	zzip_plugin_io_handlers;


U2EG_NAMESPACE_BEGIN


class ZipInStream : public FilterInStream
{
public:
    ZipInStream(const String& name, ZZIP_FILE* zzipFile, size_t uncompressedSize);
    virtual ~ZipInStream();

    virtual size_t read(u2byte* s, size_t n) override;

    virtual u2sszie_t skip(u2sszie_t count) override;

	virtual void seek(size_t pos) override;

	virtual size_t tell(void) const override;

    virtual bool eof() const override;

    virtual void close() override;

private:
	ZipInStream(const String& name, va_list argp);

protected:
	ZZIP_FILE* mZzipFile;
	/// We need caching because sometimes serializers step back in data stream and zziplib behaves slow
	StaticCache<2 * U2_STREAM_TEMP_SIZE> mCache;
};


// zip output stream not supported
/*
class ZipOutStream : public OutStream
{
public:
    ZipOutStream(const String& name, FILE* handle);
    virtual ~ZipOutStream();

    virtual size_t write(const u2byte* s, size_t n) override;

    virtual void close() override;

private:
	ZipOutStream(const String& name, va_list argp);

protected:
	FILE*      mFileHandle;

};
*/


U2EG_NAMESPACE_END

#endif /* defined(__U2ZipStream__) */
