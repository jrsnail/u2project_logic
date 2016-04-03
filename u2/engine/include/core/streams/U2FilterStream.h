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


class FilterInQueue : public FilterInStream
{
public:
    typedef std::shared_ptr<InStream>       InStreamPtr;

public:
    FilterInQueue(const String& name);
    FilterInQueue(const String& name, va_list argp);
    virtual ~FilterInQueue();

    virtual size_t read(u2byte* s, size_t n) override;

    virtual u2sszie_t skip(u2sszie_t count) override;

	virtual void seek(size_t pos) override;

	virtual size_t tell(void) const override;

    virtual void close() override;

    template <class T>
    void push(const String& name, ...);

    template <class T>
    InStreamPtr& operator->();

protected:
    InStreamPtr    m_OutterStreams;
    InStreamPtr    m_InnerStreams;
};


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
void FilterInQueue::push(const String& name, ...)
{
    va_list argp;
    va_start(argp, name);
    InStream* pIn = U2_NEW T(name, argp);
    InStreamPtr thisStream = std::make_shared<InStream>(pIn);
    va_end(argp);


    if (m_OutterStreams.get())
    {
        FilterInStream* pFilter = dynamic_cast<FilterInStream*>(m_InnerStreams.get());
        if (pFilter != nullptr)
        {
            pFilter->connect(thisStream);
        }
        m_InnerStreams = thisStream;
    }
    else
    {
        m_OutterStreams = thisStream;
        m_InnerStreams = m_OutterStreams;
    }
    
}
//-----------------------------------------------------------------------
template <class T>
FilterInQueue::InStreamPtr& FilterInQueue::operator->()
{
    return m_OutterStreams;
}



class FilterOutQueue : public FilterOutStream
{
public:
    typedef std::shared_ptr<OutStream>       OutStreamPtr;

public:
    FilterOutQueue(const String& name);
    FilterOutQueue(const String& name, va_list argp);
    virtual ~FilterOutQueue();

    virtual size_t write(const u2byte* s, size_t n) override;

    virtual void close() override;

    template <class T>
    void push(const String& name, ...);

    template <class T>
    OutStreamPtr& operator->();

protected:
    OutStreamPtr    m_OutterStreams;
    OutStreamPtr    m_InnerStreams;
};


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
void FilterOutQueue::push(const String& name, ...)
{
    va_list argp;
    va_start(argp, name);
    OutStream* pOut = U2_NEW T(name, argp);
    OutStreamPtr thisStream = std::make_shared<OutStream>(pOut);
    va_end(argp);


    if (m_OutterStreams.get())
    {
        FilterOutStream* pFilter = dynamic_cast<FilterOutStream*>(m_InnerStreams.get());
        if (pFilter != nullptr)
        {
            pFilter->connect(thisStream);
        }
        m_InnerStreams = thisStream;
    }
    else
    {
        m_OutterStreams = thisStream;
        m_InnerStreams = m_OutterStreams;
    }

}
//-----------------------------------------------------------------------
template <class T>
FilterOutQueue::OutStreamPtr& FilterOutQueue::operator->()
{
    return m_OutterStreams;
}



U2EG_NAMESPACE_END

#endif /* defined(__U2FilterStream__) */
