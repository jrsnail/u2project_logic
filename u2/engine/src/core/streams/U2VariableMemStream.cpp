#include "U2VariableMemStream.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
VariableMemInStream::VariableMemInStream(const String& name, vector<u2byte>::type* vec)
: InStream(GET_OBJECT_TYPE(VariableMemInStream), name)
{
	open(vec);
}
//-----------------------------------------------------------------------
VariableMemInStream::VariableMemInStream(const String& name, va_list argp)
    : InStream(GET_OBJECT_TYPE(VariableMemInStream), name)
{
    vector<u2byte>::type* pData = va_arg(argp, vector<u2byte>::type*);
	open(pData);
}
//-----------------------------------------------------------------------
VariableMemInStream::~VariableMemInStream()
{
    close();
}
//-----------------------------------------------------------------------
void VariableMemInStream::open(vector<u2byte>::type* vec)
{
    m_pData = vec;
    m_uPos = m_uStart = 0;
	m_uSize = m_pData->size();
    m_uEnd = m_uSize;
	assert(m_uEnd >= m_uPos);
}
//-----------------------------------------------------------------------
size_t VariableMemInStream::read(u2byte* s, size_t n)
{
	size_t cnt = n;
	// Read over end of memory?
	if (m_uPos + cnt > m_uEnd)
		cnt = m_uEnd - m_uPos;
	if (cnt == 0)
		return 0;

	assert(cnt <= n);

	memcpy(s, m_pData->data() + m_uPos, cnt);
    m_uPos += cnt;
	return cnt;
}
//-----------------------------------------------------------------------
u2sszie_t VariableMemInStream::skip(u2sszie_t count)
{
	size_t uLastPos = m_uPos;
    m_uPos = std::min(std::max((size_t)(m_uPos + count), m_uStart), m_uEnd);
	return m_uPos - uLastPos;
}
//-----------------------------------------------------------------------
void VariableMemInStream::seek(size_t pos)
{
	assert(m_uStart + pos <= m_uEnd);
    m_uPos = m_uStart + pos;
}
//-----------------------------------------------------------------------
size_t VariableMemInStream::tell(void) const
{
	return m_uPos - m_uStart;
}
//-----------------------------------------------------------------------
bool VariableMemInStream::eof() const
{
	return m_uPos >= m_uEnd;
}
//-----------------------------------------------------------------------
void VariableMemInStream::close()
{
// 	if (mFreeOnClose && m_pData)
// 	{
// 		U2_FREE(m_pData, MEMCATEGORY_GENERAL);
		m_pData = 0;
//	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
VariableMemOutStream::VariableMemOutStream(const String& name, vector<u2byte>::type* vec)
	: OutStream(GET_OBJECT_TYPE(VariableMemOutStream), name)
{
	open(vec);
}
//-----------------------------------------------------------------------
VariableMemOutStream::VariableMemOutStream(const String& name, va_list argp)
	: OutStream(GET_OBJECT_TYPE(VariableMemOutStream), name)
{
    vector<u2byte>::type* pData = va_arg(argp, vector<u2byte>::type*);
	open(pData);
}
//-----------------------------------------------------------------------
VariableMemOutStream::~VariableMemOutStream()
{
	close();
}
//-----------------------------------------------------------------------
void VariableMemOutStream::open(vector<u2byte>::type* vec)
{
	m_pData = vec;
}
//-----------------------------------------------------------------------
size_t VariableMemOutStream::write(const u2byte* s, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        m_pData->push_back(*(s + i));
    }
	return n;
}
//-----------------------------------------------------------------------
void VariableMemOutStream::close()
{
// 	if (mFreeOnClose && m_pData)
// 	{
// 		U2_FREE(m_pData, MEMCATEGORY_GENERAL);
	m_pData = 0;
//	}
}