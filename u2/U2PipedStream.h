//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2PipedStream__
#define __U2PipedStream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MemoryAllocatorConfig.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN


class PipedOutStream;


class PipedInStream : public InStream
{
public:
    PipedInStream();
    PipedInStream(size_t pipeSize);
    virtual ~PipedInStream();

    void connect(PipedOutStream* src);

    void receive(u2int32 oneByte);

    void done();

    virtual u2int32 read(u2byte* s, std::streamsize n) override;

    virtual bool eof() const override;

    virtual void close() override;

protected:
    void _establishConnection();

protected:
    const static size_t PIPE_SIZE = 1024;

protected:
    size_t          m_uPipeSize;
    bool            m_bIsConnected;
    u2char*         m_pBuffer;
};


class PipedOutStream : public OutStream
{
public:
};


U2EG_NAMESPACE_END

#endif /* defined(__U2PipedStream__) */
