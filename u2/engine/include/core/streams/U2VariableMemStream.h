#ifndef __U2VariableMemStream__
#define __U2VariableMemStream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Stream.h"


U2EG_NAMESPACE_BEGIN


class VariableMemInStream : public InStream
{
public:
    VariableMemInStream(const String& name, vector<u2byte>::type* vec);
    VariableMemInStream(const String& name, va_list argp);
    virtual ~VariableMemInStream();

    virtual size_t read(u2byte* s, size_t n) override;

    virtual u2sszie_t skip(u2sszie_t count) override;

	virtual void seek(size_t pos) override;

	virtual size_t tell(void) const override;

    virtual bool eof() const override;

    virtual void close() override;

    void open(vector<u2byte>::type* vec);

protected:
	/// Pointer to the start of the data area
    vector<u2byte>::type* m_pData;
	/// Pointer to the current position in the string
	size_t m_uPos;
	/// Pointer to the start of the string
	size_t m_uStart;
	/// Pointer to the end of the string
	size_t m_uEnd;
};


class VariableMemOutStream : public OutStream
{
public:
    VariableMemOutStream(const String& name, vector<u2byte>::type* vec);
    VariableMemOutStream(const String& name, va_list argp);
	virtual ~VariableMemOutStream();

	virtual size_t write(const u2byte* s, size_t n) override;

	virtual void close() override;

	void open(vector<u2byte>::type* vec);

protected:
	/// Pointer to the start of the data area
    vector<u2byte>::type* m_pData;
};



U2EG_NAMESPACE_END

#endif /* defined(__U2VariableMemStream__) */
