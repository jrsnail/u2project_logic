//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2FileStream__
#define __U2FileStream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN


class FileInStream : public InStream
{
public:
    FileInStream(const String& name, const u2::String& filename, std::ios_base::openmode mode = std::ios_base::in);
    FileInStream(const String& name, va_list argp);
    virtual ~FileInStream();

    virtual u2int32 read(u2byte* s, std::streamsize n) override;

    virtual std::streamoff skip(std::streamoff count) override;

    virtual bool eof() const override;

    virtual void close() override;

    void open(const u2::String& filename, std::ios_base::openmode mode = std::ios_base::in);

protected:
    std::ifstream*      m_pFInStream;

};


class FileOutStream : public OutStream
{
public:
    FileOutStream(const String& name, const u2::String& filename, std::ios_base::openmode mode = std::ios_base::out);
    FileOutStream(const String& name, va_list argp);
    virtual ~FileOutStream();

    virtual size_t write(const u2byte* s, std::streamsize n) override;

    virtual void flush() override;

    virtual void close() override;

    void open(const u2::String& filename, std::ios_base::openmode mode = std::ios_base::in);

protected:
    std::ofstream*      m_pFOutStream;

};


U2EG_NAMESPACE_END

#endif /* defined(__U2FileStream__) */
