//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2StreamQueue__
#define __U2StreamQueue__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN



template <class T>
class InStreamQueue : public Object
{
public:
    typedef std::shared_ptr<InStream>       InStreamPtr;

public:
    InStreamQueue()
		: Object(GET_OBJECT_TYPE(InStreamQueue), BLANK)
	{
	}

    virtual ~InStreamQueue()
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
class OutStreamQueue : public Object
{
public:
    typedef std::shared_ptr<OutStream>       OutStreamPtr;

public:
    OutStreamQueue()
		: Object(GET_OBJECT_TYPE(OutStreamQueue), BLANK)
	{
	}

	virtual ~OutStreamQueue()
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

#endif /* defined(__U2StreamQueue__) */
