//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Stream__
#define __U2Stream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"


U2EG_NAMESPACE_BEGIN


class InStream : public Object
{
public:
    InStream(const String& type, const String& name);
    InStream(const String& type, const String& name, va_list argp);
    virtual ~InStream();

    /** Reads up to {@code n} bytes from this stream and stores them in
        the byte array {@code s}.
        Returns the number of bytes actually read or EOF if the end of the stream
        has been reached.
    */
    virtual u2int32 read(u2byte* s, std::streamsize n) { return 0; };

    /** Skips at most {@code off} bytes in this stream. The number of actual
        bytes skipped may be anywhere between 0 and {@code off}. If
        {@code off} is negative, this method does nothing and returns 0, but
        some subclasses may throw.
    
        <p>Note the "at most" in the description of this method: this method may
        choose to skip fewer bytes than requested. Callers should <i>always</i>
        check the return value.
    
        <p>This default implementation reads bytes into a temporary buffer. Concrete
        subclasses should provide their own implementation.
        
        @return the number of bytes actually skipped.
    */
    virtual std::streamoff skip(std::streamoff count);

    //virtual stream_offset skip(stream_offset off, std::ios_base::seekdir way) = 0;

    //virtual std::streamsize readline(u2::Char* s, std::streamsize maxCount, const u2::Char* delim = _TT("\n")) = 0;

    virtual bool eof() const { return false; };

    virtual void close() {};

};


class OutStream : public Object
{
public:
    OutStream(const String& type, const String& name);
    OutStream(const String& type, const String& name, va_list argp);
    virtual ~OutStream();

    /** Writes {@code n} bytes from the byte array {@code s} to this stream.
    @param s the buffer to be written.
    @param n the number of bytes from {@code buffer} to write to this stream.
    */
    virtual void write(const u2byte* s, std::streamsize n) {};

    /** Flushes this stream. Implementations of this method should ensure that
        any buffered data is written out. This implementation does nothing.
    */
    virtual void flush() {};

    /** Closes this stream. Implementations of this method should free any
        resources used by the stream. This implementation does nothing.
    */
    virtual void close() {};
};


U2EG_NAMESPACE_END

#endif /* defined(__U2InStream__) */
