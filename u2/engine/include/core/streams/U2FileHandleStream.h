//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2FileHandleStream__
#define __U2FileHandleStream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN


class FileHandleInStream : public InStream
{
public:
    FileHandleInStream(const String& name, FILE* handle);
	FileHandleInStream(const String& name, const char* filename, const char* mode);
	FileHandleInStream(const String& name, va_list argp);
    virtual ~FileHandleInStream();

    virtual size_t read(u2byte* s, size_t n) override;

    virtual u2sszie_t skip(u2sszie_t count) override;

	virtual void seek(size_t pos) override;

	virtual size_t tell(void) const override;

    virtual bool eof() const override;

    virtual void close() override;

	void open(const char* filename, const char* mode);

protected:
	FILE*      mFileHandle;

};


class FileHandleOutStream : public OutStream
{
public:
    FileHandleOutStream(const String& name, FILE* handle);
	FileHandleOutStream(const String& name, const char* filename, const char* mode);
	FileHandleOutStream(const String& name, va_list argp);
    virtual ~FileHandleOutStream();

    virtual size_t write(const u2byte* s, size_t n) override;

    virtual void close() override;

	void open(const char* filename, const char* mode);

protected:
	FILE*      mFileHandle;

};


U2EG_NAMESPACE_END

#endif /* defined(__U2FileHandleStream__) */
