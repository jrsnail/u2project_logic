//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2StringStream__
#define __U2StringStream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN


class StringInStream : public InStream
{
public:
	StringInStream(const String& name, const String* str, size_t start = 0, size_t end = 0);
	StringInStream(const String& name, va_list argp);
    virtual ~StringInStream();

    virtual size_t read(u2byte* s, size_t n) override;

    virtual u2sszie_t skip(u2sszie_t count) override;

	virtual void seek(size_t pos) override;

	virtual size_t tell(void) const override;

    virtual bool eof() const override;

    virtual void close() override;

    void open(const String* str, size_t start, size_t end);

protected:
	/// Pointer to the start of the data area
	String m_szData;
	/// Pointer to the current position in the string
	size_t m_uPos;
	/// Pointer to the start of the string
	size_t m_uStart;
	/// Pointer to the end of the string
	size_t m_uEnd;
};


class StringOutStream : public OutStream
{
public:
	StringOutStream(const String& name, String* str, size_t start);
	StringOutStream(const String& name, va_list argp);
	virtual ~StringOutStream();

	virtual size_t write(const u2byte* s, size_t n) override;

	virtual void close() override;

	void open(String* str, size_t start);

protected:
	/// Pointer to the start of the data area
	String* m_pszData;
	/// Pointer to the current position in the string
	size_t m_uPos;
	/// Pointer to the start of the string
	size_t m_uStart;
};



U2EG_NAMESPACE_END

#endif /* defined(__U2MemoryStream__) */
