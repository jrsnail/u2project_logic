//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2FilterStream.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
FilterInStream::FilterInStream(const String& type, const String& name)
: InStream(type, name)
{
}
//-----------------------------------------------------------------------
FilterInStream::FilterInStream(const String& type, const String& name, va_list argp)
    : InStream(type, name, argp)
{

}
//-----------------------------------------------------------------------
FilterInStream::~FilterInStream()
{
}
//-----------------------------------------------------------------------
void FilterInStream::connect(std::shared_ptr<InStream> in)
{
    m_pIn = in;
}
//-----------------------------------------------------------------------
size_t FilterInStream::read(u2byte* s, size_t n)
{
    assert(m_pIn.get());
    return m_pIn->read(s, n);
}
//-----------------------------------------------------------------------
u2sszie_t FilterInStream::skip(u2sszie_t count)
{
    assert(m_pIn.get());
    return m_pIn->skip(count);
}
//-----------------------------------------------------------------------
void FilterInStream::seek(size_t pos)
{
	assert(m_pIn.get());
	return m_pIn->seek(pos);
}
//-----------------------------------------------------------------------
size_t FilterInStream::tell(void) const
{
	assert(m_pIn.get());
	return m_pIn->tell();
}
//-----------------------------------------------------------------------
void FilterInStream::close()
{
    assert(m_pIn.get());
    return m_pIn->close();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
FilterOutStream::FilterOutStream(const String& type, const String& name)
    : OutStream(type, name)
{
}
//-----------------------------------------------------------------------
FilterOutStream::FilterOutStream(const String& type, const String& name, va_list argp)
    : OutStream(type, name, argp)
{

}
//-----------------------------------------------------------------------
FilterOutStream::~FilterOutStream()
{
}
//-----------------------------------------------------------------------
void FilterOutStream::connect(std::shared_ptr<OutStream> out)
{
    m_pOut = out;
}
//-----------------------------------------------------------------------
size_t FilterOutStream::write(const u2byte* s, size_t n)
{
    assert(m_pOut.get());
    return m_pOut->write(s, n);
}
//-----------------------------------------------------------------------
void FilterOutStream::close()
{
    assert(m_pOut.get());
    return m_pOut->close();
}