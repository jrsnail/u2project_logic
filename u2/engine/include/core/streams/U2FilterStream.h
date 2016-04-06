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


template <class T>
class FilterInQueue : public Object
{
public:
    typedef std::shared_ptr<InStream>       InStreamPtr;

public:
    FilterInQueue()
		: Object(GET_OBJECT_TYPE(FilterInQueue), BLANK)
	{
	}

    virtual ~FilterInQueue()
	{
	}

    template <class Y>
    void push(const char* name, ...)
	{
		va_list argp;
		va_start(argp, name);
		InStream* pIn = U2_NEW Y(name, argp);
		InStreamPtr thisStream(pIn);
		va_end(argp);

		// when push non first stream
		if (m_OutterStreams.get())
		{
			FilterInStream* pFilter = dynamic_cast<FilterInStream*>(thisStream.get());
			if (pFilter == nullptr)
			{
			}
			else
			{
				pFilter->connect(m_OutterStreams);
				m_OutterStreams = thisStream;
			}
		}
		// when push first stream
		else
		{
			m_OutterStreams = thisStream;
			m_InnerStreams = m_OutterStreams;
		}
	}

    T* operator->()
	{
		T* pObj = dynamic_cast<T*>(m_OutterStreams.get());
		assert(pObj != nullptr);
		return pObj;
	}

protected:
    InStreamPtr    m_OutterStreams;
    InStreamPtr    m_InnerStreams;
};



template <class T>
class FilterOutQueue : public Object
{
public:
    typedef std::shared_ptr<OutStream>       OutStreamPtr;

public:
    FilterOutQueue()
		: Object(GET_OBJECT_TYPE(FilterOutQueue), BLANK)
	{
	}

	virtual ~FilterOutQueue()
	{

	}

    template <class Y>
	void push(const char* name, ...)
	{
		va_list argp;
		va_start(argp, name);
		OutStream* pOut = U2_NEW Y(name, argp);
		OutStreamPtr thisStream(pOut);
		va_end(argp);

		// when push non first stream
		if (m_OutterStreams.get())
		{
			FilterOutStream* pFilter = dynamic_cast<FilterOutStream*>(thisStream.get());
			if (pFilter == nullptr)
			{
			}
			else
			{
				pFilter->connect(m_OutterStreams);
				m_OutterStreams = thisStream;
			}
		}
		// when push first stream
		else
		{
			m_OutterStreams = thisStream;
			m_InnerStreams = m_OutterStreams;
		}
	}

	T* operator->()
	{
		T* pObj = dynamic_cast<T*>(m_OutterStreams.get());
		assert(pObj != nullptr);
		return pObj;
	}

protected:
    OutStreamPtr    m_OutterStreams;
    OutStreamPtr    m_InnerStreams;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2FilterStream__) */
