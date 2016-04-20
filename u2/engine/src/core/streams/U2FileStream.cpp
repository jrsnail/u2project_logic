//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2FileStream.h"

#include "U2MemoryAllocatorConfig.h"
#include "U2Exception.h"
#include "U2LogManager.h"



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
FileInStream::FileInStream(const String& name, std::ifstream* s, size_t inSize)
	: InStream(GET_OBJECT_TYPE(FileInStream), name)
	, m_pFInStream(s)
{
	m_uSize = inSize;
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
	m_pFInStream = U2_NEW_T(std::ifstream, MEMCATEGORY_GENERAL)();
	m_pFInStream->open(filename, mode);
    if (!m_pFInStream->is_open())
    {
        assert(0);
    }
	else
	{
		// calculate the size
		m_pFInStream->seekg(0, std::ios_base::end);
		m_uSize = (size_t)m_pFInStream->tellg();
		m_pFInStream->seekg(0, std::ios_base::beg);
	}
}
//-----------------------------------------------------------------------
size_t FileInStream::read(u2byte* s, size_t n)
{
	m_pFInStream->read((char*)(s), static_cast<std::streamsize>(n));
    return (size_t)m_pFInStream->gcount();
}
//-----------------------------------------------------------------------
u2sszie_t FileInStream::skip(u2sszie_t count)
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
	m_pFInStream->clear(); //Clear fail status in case eof was set
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
size_t FileInStream::readLine(char* buf, size_t maxCount, const String& delim)
{
	if (delim.empty())
	{
		U2_EXCEPT(Exception::ERR_INVALIDPARAMS, "No delimiter provided",
			"FileStreamDataStream::readLine");
	}
	if (delim.size() > 1)
	{
		LogManager::getSingleton().logMessage(
			"WARNING: FileStreamDataStream::readLine - using only first delimeter");
	}
	// Deal with both Unix & Windows LFs
	bool trimCR = false;
	if (delim.at(0) == '\n')
	{
		trimCR = true;
	}
	// maxCount + 1 since count excludes terminator in getline
	m_pFInStream->getline(buf, static_cast<std::streamsize>(maxCount + 1), delim.at(0));
	size_t ret = (size_t)m_pFInStream->gcount();
	// three options
	// 1) we had an eof before we read a whole line
	// 2) we ran out of buffer space
	// 3) we read a whole line - in this case the delim character is taken from the stream but not written in the buffer so the read data is of length ret-1 and thus ends at index ret-2
	// in all cases the buffer will be null terminated for us

	if (m_pFInStream->eof())
	{
		// no problem
	}
	else if (m_pFInStream->fail())
	{
		// Did we fail because of maxCount hit? No - no terminating character
		// in included in the count in this case
		if (ret == maxCount)
		{
			// clear failbit for next time 
			m_pFInStream->clear();
		}
		else
		{
			U2_EXCEPT(Exception::ERR_INTERNAL_ERROR,
				"Streaming error occurred",
				"FileStreamDataStream::readLine");
		}
	}
	else
	{
		// we need to adjust ret because we want to use it as a
		// pointer to the terminating null character and it is
		// currently the length of the data read from the stream
		// i.e. 1 more than the length of the data in the buffer and
		// hence 1 more than the _index_ of the NULL character
		--ret;
	}

	// trim off CR if we found CR/LF
	if (trimCR && buf[ret - 1] == '\r')
	{
		--ret;
		buf[ret] = '\0';
	}
	return ret;
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
FileOutStream::FileOutStream(const String& name, std::ofstream* s)
	: OutStream(GET_OBJECT_TYPE(FileOutStream), name)
	, m_pFOutStream(s)
{

}
//-----------------------------------------------------------------------
FileOutStream::FileOutStream(const String& name, va_list argp)
    : OutStream(GET_OBJECT_TYPE(FileOutStream), name)
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
	m_pFOutStream = U2_NEW_T(std::ofstream, MEMCATEGORY_GENERAL)();
	m_pFOutStream->open(filename, mode);
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
		m_pFOutStream->flush();
        m_pFOutStream->close();

        // delete the stream too
        U2_DELETE_T(m_pFOutStream, basic_ofstream, MEMCATEGORY_GENERAL);
        m_pFOutStream = nullptr;
    }
}