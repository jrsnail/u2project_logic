//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2PipedStream.h"

#include "U2MemoryAllocatorConfig.h"
#include "U2Exception.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PipedInStream::PipedInStream(const String& type, const String& name)
: InStream(type, name)
, m_pBuffer(nullptr)
, m_nIn(-1)
, m_nOut(0)
, m_bIsConnected(false)
, m_bIsClosed(false)
, m_nPipeSize(PIPE_SIZE)
{
}
//-----------------------------------------------------------------------
PipedInStream::PipedInStream(const String& type, const String& name, va_list argp)
    : InStream(type, name)
	, m_pBuffer(nullptr)
	, m_nIn(-1)
	, m_nOut(0)
	, m_bIsConnected(false)
	, m_bIsClosed(false)
	, m_nPipeSize(PIPE_SIZE)
{

}
//-----------------------------------------------------------------------
PipedInStream::PipedInStream(size_t pipeSize)
	: InStream(GET_OBJECT_TYPE(PipedInStream), BLANK)
	, m_pBuffer(nullptr)
	, m_nIn(-1)
	, m_nOut(0)
	, m_bIsConnected(false)
	, m_bIsClosed(false)
	, m_nPipeSize(pipeSize)
{
	U2Assert(m_nPipeSize > 0, "pipe size too small");
	m_pBuffer = static_cast<u2char*>(U2_MALLOC(m_nPipeSize, MEMCATEGORY_GENERAL));
}
//-----------------------------------------------------------------------
PipedInStream::PipedInStream(PipedOutStream* outStream, size_t pipeSize)
	: PipedInStream(pipeSize)
{
	connect(outStream);
}
//-----------------------------------------------------------------------
PipedInStream::~PipedInStream()
{
}
//-----------------------------------------------------------------------
void PipedInStream::connect(PipedOutStream* outStream)
{
	U2Assert(outStream != nullptr, "nullptr");
	outStream->connect(this);
}
//-----------------------------------------------------------------------
void PipedInStream::establishConnection()
{
	U2Assert(!m_bIsConnected, "Pipe already connected");
	if (m_pBuffer == nullptr)
	{
		m_nPipeSize = PIPE_SIZE;
		m_pBuffer = static_cast<u2char*>(U2_MALLOC(m_nPipeSize, MEMCATEGORY_GENERAL));
	}
	m_bIsConnected = true;
}
//-----------------------------------------------------------------------
bool PipedInStream::isConnected()
{
	return m_bIsConnected;
}
//-----------------------------------------------------------------------
void PipedInStream::close()
{
	U2_FREE(m_pBuffer, MEMCATEGORY_GENERAL);
	m_pBuffer = nullptr;
}
//-----------------------------------------------------------------------
u2int32 PipedInStream::read(u2byte* s, std::streamsize n)
{
	if (n == 0)
	{
		return 0;
	}
	U2_LOCK_AUTO_MUTEX;
	U2Assert(m_bIsConnected, "Not connected");
	U2Assert(m_pBuffer != nullptr, "InputStream is closed");

	{
		u2int32 attempts = 3;
		while (m_nIn == -1)
		{
			// Are we at end of stream?
			if (m_bIsClosed)
			{
				return -1;
			}
			if (attempts-- <= 0) 
			{
				U2_EXCEPT(Exception::ERR_INTERNAL_ERROR, "Pipe broken.", "PipedInStream::read");
			}
			// Notify callers of receive()
			U2_THREAD_NOTIFY_ALL(m_WritableSync);
			U2_THREAD_WAIT_FOR(m_ReadableSync, lck, 1000);
		}
	}

	u2int32 totalCopied = 0;

	// copy bytes from out thru the end of buffer
	if (m_nOut >= m_nIn)
	{
		u2int32 leftInBuffer = m_nPipeSize - m_nOut;
		u2int32 length = leftInBuffer < n ? leftInBuffer : n;
		memcpy(s, m_pBuffer + m_nOut, length);
		m_nOut += length;
		if (m_nOut == m_nPipeSize)
		{
			m_nOut = 0;
		}
		if (m_nOut == m_nIn)
		{
			// empty buffer
			m_nIn = -1;
			m_nOut = 0;
		}
		totalCopied += length;
	}

	// copy bytes from out thru in
	if (totalCopied < n && m_nIn != -1)
	{
		u2int32 leftInBuffer = m_nIn - m_nOut;
		u2int32 leftToCopy = n - totalCopied;
		u2int32 length = leftToCopy < leftInBuffer ? leftToCopy : leftInBuffer;
		memcpy(s + totalCopied, m_pBuffer + m_nOut, length);
		m_nOut += length;
		if (m_nOut == m_nIn)
		{
			// empty buffer
			m_nIn = -1;
			m_nOut = 0;
		}
		totalCopied += length;
	}

	// let blocked writers write to the newly available buffer space
	U2_THREAD_NOTIFY_ALL(m_WritableSync);

	return totalCopied;
}
//-----------------------------------------------------------------------
void PipedInStream::receive(const u2byte* s, std::streamsize n)
{
	if (n == 0)
	{
		return;
	}
	U2_LOCK_AUTO_MUTEX;
	U2Assert(m_pBuffer != nullptr && !m_bIsClosed, "Pipe is closed");

	{
		while (m_pBuffer != nullptr && m_nOut == m_nIn) 
		{
			U2_THREAD_NOTIFY_ALL(m_ReadableSync);
			U2_THREAD_WAIT_FOR(m_WritableSync, lck, 1000);
		}
	}

	U2Assert(m_pBuffer != nullptr, "Pipe is closed");

	if (m_nIn == -1)
	{
		m_nIn = 0;
	}

	u2int32 totalCopied = 0;

	// copy bytes from in thru the end of buffer
	if (m_nIn >= m_nOut)
	{
		u2int32 leftInBuffer = m_nPipeSize - m_nIn;
		u2int32 length = leftInBuffer < n ? leftInBuffer : n;
		memcpy(m_pBuffer + m_nIn, s, length);
		m_nIn += length;
		if (m_nIn == m_nPipeSize)
		{
			m_nIn = 0;
		}
		totalCopied += length;
	}

	// copy bytes from in thru out
	if (totalCopied < n && m_nIn < m_nOut)
	{
		u2int32 leftInBuffer = m_nOut - m_nIn;
		u2int32 leftToCopy = n - totalCopied;
		u2int32 length = leftToCopy < leftInBuffer ? leftToCopy : leftInBuffer;
		memcpy(m_pBuffer + m_nIn, s + totalCopied, length);
		m_nIn += length;
		if (m_nIn == m_nPipeSize)
		{
			m_nIn = 0;
		}
		totalCopied += length;
	}

	U2_THREAD_NOTIFY_ALL(m_ReadableSync);
}
//-----------------------------------------------------------------------
void PipedInStream::done()
{
	U2_LOCK_AUTO_MUTEX;
	m_bIsClosed = true;
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PipedOutStream::PipedOutStream(const String& type, const String& name)
    : OutStream(type, name)
	, m_pTarget(nullptr)
{
}
//-----------------------------------------------------------------------
PipedOutStream::PipedOutStream(const String& type, const String& name, va_list argp)
    : OutStream(type, name)
	, m_pTarget(nullptr)
{

}
//-----------------------------------------------------------------------
PipedOutStream::PipedOutStream()
	: OutStream(GET_OBJECT_TYPE(PipedOutStream), BLANK)
	, m_pTarget(nullptr)
{
}
//-----------------------------------------------------------------------
PipedOutStream::PipedOutStream(PipedInStream* target)
	: OutStream(GET_OBJECT_TYPE(PipedOutStream), BLANK)
	, m_pTarget(nullptr)
{
	connect(target);
}
//-----------------------------------------------------------------------
PipedOutStream::~PipedOutStream()
{
}
//-----------------------------------------------------------------------
void PipedOutStream::close()
{
	if (m_pTarget != nullptr)
	{
		m_pTarget->done();
		m_pTarget = nullptr;
	}
}
//-----------------------------------------------------------------------
void PipedOutStream::connect(PipedInStream* stream)
{
	U2Assert(stream != nullptr, "stream == nullptr");
	U2_LOCK_AUTO_MUTEX;
	U2Assert(m_pTarget != nullptr, "Already connected");
	U2Assert(!m_pTarget->isConnected(), "Pipe already connected");
	stream->establishConnection();
	m_pTarget = stream;
}
//-----------------------------------------------------------------------
void PipedOutStream::flush()
{
	U2_LOCK_AUTO_MUTEX;

	if (m_pTarget == nullptr)
	{
		return;
	}
	//m_pTarget->notifyAll();
}
//-----------------------------------------------------------------------
void PipedOutStream::write(const u2byte* s, std::streamsize n)
{
	U2Assert(m_pTarget != nullptr, "Pipe not connected");
	m_pTarget->receive(s, n);
}