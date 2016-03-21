//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2FileStream.h"

#include "U2MemoryAllocatorConfig.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
FileInStream::FileInStream(const String& name, const u2::String& filename, std::ios_base::openmode mode)
: InStream(GET_OBJECT_TYPE(FileInStream), name)
, m_pFInStream(nullptr)
{
    open(filename, mode);
}
//-----------------------------------------------------------------------
FileInStream::FileInStream(const String& name, va_list argp)
    : InStream(GET_OBJECT_TYPE(FileInStream), name)
    , m_pFInStream(nullptr)
{
    u2::String filename = va_arg(argp, const char*);
    std::ios_base::openmode mode = va_arg(argp, std::ios_base::openmode);
    open(filename, mode);
}
//-----------------------------------------------------------------------
FileInStream::~FileInStream()
{
    close();
}
//-----------------------------------------------------------------------
void FileInStream::open(const u2::String& filename, std::ios_base::openmode mode)
{
    if (m_pFInStream != nullptr)
    {
        assert(0);
    }
    if ((mode & std::ios_base::out)
        || (mode & std::ios_base::ate) 
        || (mode & std::ios_base::app)
        || (mode & std::ios_base::trunc))
    {
        assert(0);
    }
    m_pFInStream = new std::ifstream(filename, mode);
    if (!m_pFInStream->is_open())
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
size_t FileInStream::read(u2byte* s, size_t n)
{
    m_pFInStream->read((char*)s, n);
    return (u2int32)m_pFInStream->gcount();
}
//-----------------------------------------------------------------------
ssize_t FileInStream::skip(ssize_t count)
{
#if defined(STLPORT)
    // Workaround for STLport issues: After reached eof of file stream,
    // it's seems the stream was putted in intermediate state, and will be
    // fail if try to repositioning relative to current position.
    // Note: tellg() fail in this case too.
    if (m_pFInStream->eof())
    {
        m_pFInStream->clear();
        // Use seek relative to either begin or end to bring the stream
        // back to normal state.
        m_pFInStream->seekg(0, std::ios::end);
    }
#endif
    std::streampos nStart = m_pFInStream->tellg();
    ->clear(); //Clear fail status in case eof was set
    m_pFInStream->seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
    return m_pFInStream->tellg() - nStart;
}
//-----------------------------------------------------------------------
void FileInStream::seek(size_t pos)
{
	m_pFInStream->clear(); //Clear fail status in case eof was set
	m_pFInStream->seekg(static_cast<std::streamoff>(pos), std::ios::beg);
}
//-----------------------------------------------------------------------
size_t FileInStream::tell(void) const
{
	m_pFInStream->clear(); //Clear fail status in case eof was set
	return (size_t)m_pFInStream->tellg();
}
//-----------------------------------------------------------------------
bool FileInStream::eof() const
{
    return m_pFInStream->eof();
}
//-----------------------------------------------------------------------
void FileInStream::close()
{
    if (m_pFInStream)
    {
        m_pFInStream->close();

        // delete the stream too
        U2_DELETE_T(m_pFInStream, basic_ifstream, MEMCATEGORY_GENERAL);
        m_pFInStream = nullptr;
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
FileOutStream::FileOutStream(const String& name, const u2::String& filename, std::ios_base::openmode mode)
    : OutStream(GET_OBJECT_TYPE(FileOutStream), name)
    , m_pFOutStream(nullptr)
{
    open(filename, mode);
}
//-----------------------------------------------------------------------
FileOutStream::FileOutStream(const String& name, va_list argp)
    : OutStream(GET_OBJECT_TYPE(FileInStream), name)
    , m_pFOutStream(nullptr)
{
    u2::String filename = va_arg(argp, const char*);
    std::ios_base::openmode mode = va_arg(argp, std::ios_base::openmode);
    open(filename, mode);
}
//-----------------------------------------------------------------------
FileOutStream::~FileOutStream()
{
    close();
}
//-----------------------------------------------------------------------
void FileOutStream::open(const u2::String& filename, std::ios_base::openmode mode)
{
    if (m_pFOutStream != nullptr)
    {
        assert(0);
    }
    if (mode & std::ios_base::in)
    {
        assert(0);
    }
    m_pFOutStream = new std::ofstream(filename, mode);
    if (!m_pFOutStream->is_open())
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
size_t FileOutStream::write(const u2byte* s, size_t n)
{
	size_t written = 0;
	if (m_pFOutStream)
	{
		m_pFOutStream->write((const char*)s, n);
		written = n;
	}
	return written;
}
//-----------------------------------------------------------------------
void FileOutStream::close()
{
    if (m_pFOutStream)
    {
        m_pFOutStream->close();

        // delete the stream too
        U2_DELETE_T(m_pFOutStream, basic_ofstream, MEMCATEGORY_GENERAL);
        m_pFOutStream = nullptr;
    }
}