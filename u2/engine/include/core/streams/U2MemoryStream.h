//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2MemoryStream__
#define __U2MemoryStream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN


class MemoryInStream : public InStream
{
public:
	MemoryInStream(const String& name, void* pMem, size_t inSize);
	MemoryInStream(const String& name, va_list argp);
    virtual ~MemoryInStream();

    virtual size_t read(u2byte* s, size_t n) override;

    virtual u2sszie_t skip(u2sszie_t count) override;

	virtual void seek(size_t pos) override;

	virtual size_t tell(void) const override;

    virtual bool eof() const override;

    virtual void close() override;

    void open(void* pMem, size_t inSize);

protected:
	/// Pointer to the start of the data area
	u2uchar* m_pData;
	/// Pointer to the current position in the memory
	u2uchar* m_pPos;
	/// Pointer to the end of the memory
	u2uchar* m_pEnd;
};


class MemoryOutStream : public OutStream
{
public:
	MemoryOutStream(const String& name, void* pMem, size_t inSize);
	MemoryOutStream(const String& name, va_list argp);
	virtual ~MemoryOutStream();

	virtual size_t write(const u2byte* s, size_t n) override;

	virtual void close() override;

	void open(void* pMem, size_t inSize);

protected:
	/// Pointer to the start of the data area
	u2uchar* m_pData;
	/// Pointer to the current position in the memory
	u2uchar* m_pPos;
	/// Pointer to the end of the memory
	u2uchar* m_pEnd;
	/// Size of the data in the stream (may be 0 if size cannot be determined)
	size_t	 m_uSize;
};



U2EG_NAMESPACE_END

#endif /* defined(__U2MemoryStream__) */
