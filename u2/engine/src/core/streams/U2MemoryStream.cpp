//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2MemoryStream.h"

#include "U2MemoryAllocatorConfig.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
MemoryInStream::MemoryInStream(const String& name, void* pMem, size_t inSize)
: InStream(GET_OBJECT_TYPE(MemoryInStream), name)
{
	open(pMem, inSize);
}
//-----------------------------------------------------------------------
MemoryInStream::MemoryInStream(const String& name, std::shared_ptr<u2byte> pMemPtr, size_t inSize)
	: InStream(GET_OBJECT_TYPE(MemoryInStream), name)
	, m_MemPtr(pMemPtr)
{
	open(m_MemPtr.get(), inSize);
}
//-----------------------------------------------------------------------
MemoryInStream::MemoryInStream(const String& name, va_list argp)
    : InStream(GET_OBJECT_TYPE(MemoryInStream), name)
{
	void* pMem = va_arg(argp, void*);
	size_t inSize = va_arg(argp, size_t);
	open(pMem, inSize);
}
//-----------------------------------------------------------------------
MemoryInStream::~MemoryInStream()
{
    close();
}
//-----------------------------------------------------------------------
void MemoryInStream::open(void* pMem, size_t inSize)
{
	m_pData = m_pPos = static_cast<u2uchar*>(pMem);
	m_uSize = inSize;
	m_pEnd = m_pData + m_uSize;
	assert(m_pEnd >= m_pPos);
}
//-----------------------------------------------------------------------
size_t MemoryInStream::read(u2byte* s, size_t n)
{
	size_t cnt = n;
	// Read over end of memory?
	if (m_pPos + cnt > m_pEnd)
		cnt = m_pEnd - m_pPos;
	if (cnt == 0)
		return 0;

	assert(cnt <= n);

	memcpy(s, m_pPos, cnt);
	m_pPos += cnt;
	return cnt;
}
//-----------------------------------------------------------------------
u2sszie_t MemoryInStream::skip(u2sszie_t count)
{
	u2uchar* pLastPos = m_pPos;
	m_pPos = std::min(std::max(m_pPos + count, m_pData), m_pEnd);
	return m_pPos - pLastPos;
}
//-----------------------------------------------------------------------
void MemoryInStream::seek(size_t pos)
{
	assert(m_pData + pos <= m_pEnd);
	m_pPos = m_pData + pos;
}
//-----------------------------------------------------------------------
size_t MemoryInStream::tell(void) const
{
	//m_pData is start, m_pPos is current location
	return m_pPos - m_pData;
}
//-----------------------------------------------------------------------
bool MemoryInStream::eof() const
{
	return m_pPos >= m_pEnd;
}
//-----------------------------------------------------------------------
void MemoryInStream::close()
{
// 	if (mFreeOnClose && m_pData)
// 	{
// 		U2_FREE(m_pData, MEMCATEGORY_GENERAL);
		m_pData = 0;
//	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
MemoryOutStream::MemoryOutStream(const String& name, void* pMem, size_t inSize)
	: OutStream(GET_OBJECT_TYPE(MemoryOutStream), name)
{
	open(pMem, inSize);
}
//-----------------------------------------------------------------------
MemoryOutStream::MemoryOutStream(const String& name, va_list argp)
	: OutStream(GET_OBJECT_TYPE(MemoryOutStream), name)
{
	void* pMem = va_arg(argp, void*);
	size_t inSize = va_arg(argp, size_t);
	open(pMem, inSize);
}
//-----------------------------------------------------------------------
MemoryOutStream::~MemoryOutStream()
{
	close();
}
//-----------------------------------------------------------------------
void MemoryOutStream::open(void* pMem, size_t inSize)
{
	m_pData = m_pPos = static_cast<u2uchar*>(pMem);
	m_uSize = inSize;
	m_pEnd = m_pData + m_uSize;
	assert(m_pEnd >= m_pPos);
}
//-----------------------------------------------------------------------
size_t MemoryOutStream::write(const u2byte* s, size_t n)
{
	size_t written = 0;
	{
		written = n;
		// we only allow writing within the extents of allocated memory
		// check for buffer overrun & disallow
		if (m_pPos + written > m_pEnd)
			written = m_pEnd - m_pPos;
		if (written == 0)
			return 0;

		memcpy(m_pPos, s, written);
		m_pPos += written;
	}
	return written;
}
//-----------------------------------------------------------------------
void MemoryOutStream::close()
{
// 	if (mFreeOnClose && m_pData)
// 	{
// 		U2_FREE(m_pData, MEMCATEGORY_GENERAL);
	m_pData = 0;
//	}
}