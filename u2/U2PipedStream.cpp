//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2PipedStream.h"

#include "U2ThreadHeaders.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PipedInStream::PipedInStream()
: InStream(GET_OBJECT_TYPE(PipedInStream), BLANK)
, m_uPipeSize(PIPE_SIZE)
, m_bIsConnected(false)
, m_pBuffer(nullptr)
{

}
//-----------------------------------------------------------------------
PipedInStream::PipedInStream(size_t pipeSize)
    : InStream(GET_OBJECT_TYPE(PipedInStream), BLANK)
    , m_uPipeSize(pipeSize)
    , m_bIsConnected(false)
    , m_pBuffer(nullptr)
{

}
//-----------------------------------------------------------------------
PipedInStream::~PipedInStream()
{
    if (m_pBuffer != nullptr)
    {
        U2_FREE(m_pBuffer, MEMCATEGORY_GENERAL);
    }
}
//-----------------------------------------------------------------------
void PipedInStream::connect(PipedOutStream* src)
{
    src->connect(this);
}
//-----------------------------------------------------------------------
void PipedInStream::_establishConnection()
{
    if (m_bIsConnected)
    {
        assert(0);
    }

    if (m_pBuffer == nullptr)
    {
        m_pBuffer = static_cast<u2char*>(U2_MALLOC(m_uPipeSize, MEMCATEGORY_GENERAL));
    }
}
//-----------------------------------------------------------------------
u2int32 PipedInStream::read(u2byte* s, std::streamsize n)
{
    if (n == 0)
    {
        return 0;
    }

    if (!m_bIsConnected)
    {
        assert(0);
    }

    if (m_pBuffer == nullptr)
    {
        assert(0);
    }

    std::this_thread::get_id();
}
//-----------------------------------------------------------------------
bool PipedInStream::eof() const
{

}
//-----------------------------------------------------------------------
void PipedInStream::close()
{

}
//-----------------------------------------------------------------------
void PipedInStream::receive(u2int32 oneByte)
{

}
//-----------------------------------------------------------------------
void PipedInStream::done()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------