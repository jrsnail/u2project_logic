//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2StringStream.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
StringInStream::StringInStream(const String& name, const String* str, size_t start, size_t end)
: InStream(GET_OBJECT_TYPE(StringInStream), name)
{
	open(str, start, end);
}
//-----------------------------------------------------------------------
StringInStream::StringInStream(const String& name, va_list argp)
    : InStream(GET_OBJECT_TYPE(StringInStream), name)
{
	const String* pStr = va_arg(argp, const String*);
	size_t uStart = va_arg(argp, size_t);
	size_t uEnd = va_arg(argp, size_t);
	open(pStr, uStart, uEnd);
}
//-----------------------------------------------------------------------
StringInStream::~StringInStream()
{
    close();
}
//-----------------------------------------------------------------------
void StringInStream::open(const String* str, size_t start, size_t end)
{
	m_szData = *str;
	m_uStart = start;
	m_uEnd = (end == 0) ? str->size() : end;
	if (m_uEnd > m_szData.size())
	{
		m_uEnd = m_szData.size();
	}
	m_uPos = m_uStart;

	assert(m_uStart < m_szData.size());
	assert(m_uEnd > m_uStart);

	m_uSize = m_uEnd - m_uStart;
}
//-----------------------------------------------------------------------
size_t StringInStream::read(u2byte* s, size_t n)
{
	size_t cnt = n;
	// Read over end of memory?
	if (m_uPos + cnt > m_uEnd)
		cnt = m_uEnd - m_uPos;
	if (cnt == 0)
		return 0;

	assert(cnt <= n);

	memcpy(s, m_szData.c_str() + m_uPos, cnt);
	m_uPos += cnt;
	return cnt;
}
//-----------------------------------------------------------------------
u2sszie_t StringInStream::skip(u2sszie_t count)
{
	size_t uLastPos = m_uPos;
	m_uPos = std::min((size_t)std::max((u2sszie_t)m_uPos + count, (u2sszie_t)m_uStart), m_uEnd);
	return m_uPos - uLastPos;
}
//-----------------------------------------------------------------------
void StringInStream::seek(size_t pos)
{
	assert(m_uStart + pos <= m_uEnd);
	m_uPos = m_uStart + pos;
}
//-----------------------------------------------------------------------
size_t StringInStream::tell(void) const
{
	//m_pData is start, m_pPos is current location
	return m_uPos - m_uStart;
}
//-----------------------------------------------------------------------
bool StringInStream::eof() const
{
	return m_uPos >= m_uEnd;
}
//-----------------------------------------------------------------------
void StringInStream::close()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
StringOutStream::StringOutStream(const String& name, String* str, size_t start)
	: OutStream(GET_OBJECT_TYPE(StringOutStream), name)
{
	open(str, start);
}
//-----------------------------------------------------------------------
StringOutStream::StringOutStream(const String& name, va_list argp)
	: OutStream(GET_OBJECT_TYPE(StringOutStream), name)
{
	String* pStr = va_arg(argp, String*);
	size_t uStart = va_arg(argp, size_t);
	open(pStr, uStart);
}
//-----------------------------------------------------------------------
StringOutStream::~StringOutStream()
{
	close();
}
//-----------------------------------------------------------------------
void StringOutStream::open(String* str, size_t start)
{
	m_pszData = str;
	m_uStart = start;
	m_uPos = m_uStart;

	assert(m_uStart < m_pszData->size());
}
//-----------------------------------------------------------------------
size_t StringOutStream::write(const u2byte* s, size_t n)
{
	size_t written = 0;
	for (; m_uPos < m_pszData->size() && written < n; m_uPos++, written++)
	{
		m_pszData[m_uPos] = *(s + written);
	}
	if (written < n)
	{
		m_pszData->append((const char*)s + written);
	}
	return n;
}
//-----------------------------------------------------------------------
void StringOutStream::close()
{
	m_pszData = nullptr;
}