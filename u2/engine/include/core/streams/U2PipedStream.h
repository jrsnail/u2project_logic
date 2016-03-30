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
#include "U2Object.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN


class PipedOutStream;


class PipedInStream : public InStream
{
protected:
	/**
	* The size of the default pipe in bytes.
	*/
	static const u2int32 PIPE_SIZE = 1024;

public:
	PipedInStream(size_t pipeSize = PIPE_SIZE);
	PipedInStream(PipedOutStream* outStream, size_t pipeSize = PIPE_SIZE);
    virtual ~PipedInStream();

	void connect(PipedOutStream* outStream);
	void establishConnection();
	bool isConnected();

	/**
	* Closes this stream. This implementation releases the buffer used for the
	* pipe and notifies all threads waiting to read or write.
	*/
	virtual void close() override;
	virtual u2int32 read(u2byte* s, std::streamsize n) override;
	void receive(const u2byte* s, std::streamsize n);
	void done();

private:
	PipedInStream(const String& type, const String& name);
	PipedInStream(const String& type, const String& name, va_list argp);

	PipedInStream(const PipedInStream& rhs) = delete;
	PipedInStream& operator=(const PipedInStream& rhs) = delete;
	PipedInStream(PipedInStream&& rhs) = delete;
	PipedInStream& operator=(PipedInStream&& rhs) = delete;

protected:
	U2_AUTO_MUTEX;
	U2_THREAD_SYNCHRONISER(m_ReadableSync);
	U2_THREAD_SYNCHRONISER(m_WritableSync);

	/**
	* The circular buffer through which data is passed. Data is read from the
	* range {@code [out, in)} and written to the range {@code [in, out)}.
	* Data in the buffer is either sequential: <pre>
	*     { - - - X X X X X X X - - - - - }
	*             ^             ^
	*             |             |
	*            out           in</pre>
	* ...or wrapped around the buffer's end: <pre>
	*     { X X X X - - - - - - - - X X X }
	*               ^               ^
	*               |               |
	*              in              out</pre>
	* When the buffer is empty, {@code in == -1}. Reading when the buffer is
	* empty will block until data is available. When the buffer is full,
	* {@code in == out}. Writing when the buffer is full will block until free
	* space is available.
	*/
	u2char* m_pBuffer;

	/**
	* The index in {@code buffer} where the next byte will be written.
	*/
	u2int32 m_nIn;

	/**
	* The index in {@code buffer} where the next byte will be read.
	*/
	u2int32 m_nOut;

	/**
	* Indicates if this pipe is connected.
	*/
	bool m_bIsConnected;
	bool m_bIsClosed;
	u2int32 m_nPipeSize;
};


class PipedOutStream : public OutStream
{
public:
	PipedOutStream();
	PipedOutStream(PipedInStream* target);
    virtual ~PipedOutStream();

	virtual void close() override;
	void connect(PipedInStream* stream);
	virtual void flush() override;
	virtual void write(const u2byte* s, std::streamsize n) override;

private:
	PipedOutStream(const String& type, const String& name);
	PipedOutStream(const String& type, const String& name, va_list argp);

	PipedOutStream(const PipedOutStream& rhs) = delete;
	PipedOutStream& operator=(const PipedOutStream& rhs) = delete;
	PipedOutStream(PipedOutStream&& rhs) = delete;
	PipedOutStream& operator=(PipedOutStream&& rhs) = delete;

protected:
	U2_AUTO_MUTEX;
	PipedInStream*		m_pTarget;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2InStream__) */
