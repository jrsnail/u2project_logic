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
u2int32 FilterInStream::read(u2byte* s, std::streamsize n)
{
    assert(m_pIn.get());
    return m_pIn->read(s, n);
}
//-----------------------------------------------------------------------
std::streamoff FilterInStream::skip(std::streamoff off)
{
    assert(m_pIn.get());
    return m_pIn->skip(off);
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
size_t FilterOutStream::write(const u2byte* s, std::streamsize n)
{
    assert(m_pOut.get());
    return m_pOut->write(s, n);
}
//-----------------------------------------------------------------------
void FilterOutStream::flush()
{
    assert(m_pOut.get());
    return m_pOut->flush();
}
//-----------------------------------------------------------------------
void FilterOutStream::close()
{
    assert(m_pOut.get());
    return m_pOut->close();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
FilterInQueue::FilterInQueue(const String& name)
    : FilterInStream(GET_OBJECT_TYPE(FilterInQueue), name)
{
}
//-----------------------------------------------------------------------
FilterInQueue::FilterInQueue(const String& name, va_list argp)
    : FilterInStream(GET_OBJECT_TYPE(FilterInQueue), name, argp)
{

}
//-----------------------------------------------------------------------
FilterInQueue::~FilterInQueue()
{
}
//-----------------------------------------------------------------------
u2int32 FilterInQueue::read(u2byte* s, std::streamsize n)
{
    if (m_OutterStreams.get())
    {
        return m_OutterStreams->read(s, n);
    }
    else
    {
        return 0;
    }
}
//-----------------------------------------------------------------------
std::streamoff FilterInQueue::skip(std::streamoff off)
{
    if (m_OutterStreams.get())
    {
        return m_OutterStreams->skip(off);
    }
    else
    {
        return 0;
    }
}
//-----------------------------------------------------------------------
void FilterInQueue::close()
{
    if (m_OutterStreams.get())
    {
        m_OutterStreams->close();
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
FilterOutQueue::FilterOutQueue(const String& name)
    : FilterOutStream(GET_OBJECT_TYPE(FilterOutQueue), name)
{
}
//-----------------------------------------------------------------------
FilterOutQueue::FilterOutQueue(const String& name, va_list argp)
    : FilterOutStream(GET_OBJECT_TYPE(FilterOutQueue), name, argp)
{

}
//-----------------------------------------------------------------------
FilterOutQueue::~FilterOutQueue()
{
}
//-----------------------------------------------------------------------
void FilterOutQueue::write(const u2byte* s, std::streamsize n)
{
    if (m_InnerStreams.get())
    {
        return m_InnerStreams->write(s, n);
    }
}
//-----------------------------------------------------------------------
void FilterOutQueue::flush()
{
    if (m_InnerStreams.get())
    {
        return m_InnerStreams->flush();
    }
}
//-----------------------------------------------------------------------
void FilterOutQueue::close()
{
    if (m_InnerStreams.get())
    {
        return m_InnerStreams->close();
    }
}