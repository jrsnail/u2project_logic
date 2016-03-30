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
    virtual ~FileHandleInStream();

    virtual u2int32 read(u2byte* s, std::streamsize n) override;

    virtual std::streamoff skip(std::streamoff count) override;

    virtual bool eof() const override;

    virtual void close() override;

private:
	FileHandleInStream(const String& name, va_list argp);

protected:
	FILE*      mFileHandle;

};


class FileHandleOutStream : public OutStream
{
public:
    FileHandleOutStream(const String& name, FILE* handle);
    virtual ~FileHandleOutStream();

    virtual size_t write(const u2byte* s, std::streamsize n) override;

    virtual void flush() override;

    virtual void close() override;

private:
	FileHandleOutStream(const String& name, va_list argp);

protected:
	FILE*      mFileHandle;

};


U2EG_NAMESPACE_END

#endif /* defined(__U2FileHandleStream__) */
