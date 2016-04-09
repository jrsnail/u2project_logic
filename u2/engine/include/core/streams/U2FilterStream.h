//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2FilterStream__
#define __U2FilterStream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN


class FilterInStream : public InStream
{
public:
    FilterInStream(const String& type, const String& name);
    FilterInStream(const String& type, const String& name, va_list argp);
    virtual ~FilterInStream();

    void connect(std::shared_ptr<InStream> in);

    virtual size_t read(u2byte* s, size_t n) override;

    virtual u2sszie_t skip(u2sszie_t count) override;

	virtual void seek(size_t pos) override;

	virtual size_t tell(void) const override;

    virtual void close() override;

protected:
    std::shared_ptr<InStream> m_pIn;

};


class FilterOutStream : public OutStream
{
public:
    FilterOutStream(const String& type, const String& name);
    FilterOutStream(const String& type, const String& name, va_list argp);
    virtual ~FilterOutStream();

    void connect(std::shared_ptr<OutStream> out);

    virtual size_t write(const u2byte* s, size_t n) override;

    virtual void close() override;

protected:
    std::shared_ptr<OutStream> m_pOut;

};


U2EG_NAMESPACE_END

#endif /* defined(__U2FilterStream__) */
