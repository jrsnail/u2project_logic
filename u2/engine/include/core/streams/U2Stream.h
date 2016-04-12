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


/** Template version of cache based on static array.
'cacheSize' defines size of cache in bytes. */
template <size_t cacheSize>
class StaticCache
{
protected:
	/// Static buffer
	u2char mBuffer[cacheSize];
	/// Number of bytes valid in cache (written from the beginning of static buffer)
	size_t mValidBytes;
	/// Current read position
	size_t mPos;

public:
	/// Constructor
	StaticCache()
	{
		mValidBytes = 0;
		mPos = 0;
		memset(mBuffer, 0, cacheSize);
	}

	/** Cache data pointed by 'buf'. If 'count' is greater than cache size, we cache only last bytes.
	Returns number of bytes written to cache. */
	size_t cacheData(const void* buf, size_t count)
	{
		assert(avail() == 0 && "It is assumed that you cache data only after you have read everything.");

		if (count < cacheSize)
		{
			// number of bytes written is less than total size of cache
			if (count + mValidBytes <= cacheSize)
			{
				// just append
				memcpy(mBuffer + mValidBytes, buf, count);
				mValidBytes += count;
			}
			else
			{
				size_t begOff = count - (cacheSize - mValidBytes);
				// override old cache content in the beginning
				memmove(mBuffer, mBuffer + begOff, mValidBytes - begOff);
				// append new data
				memcpy(mBuffer + cacheSize - count, buf, count);
				mValidBytes = cacheSize;
			}
			mPos = mValidBytes;
			return count;
		}
		else
		{
			// discard all
			memcpy(mBuffer, (const u2char*)buf + count - cacheSize, cacheSize);
			mValidBytes = mPos = cacheSize;
			return cacheSize;
		}
	}

	/** Read data from cache to 'buf' (maximum 'count' bytes). Returns number of bytes read from cache. */
	size_t read(void* buf, size_t count)
	{
		size_t rb = avail();
		rb = (rb < count) ? rb : count;
		memcpy(buf, mBuffer + mPos, rb);
		mPos += rb;
		return rb;
	}

	/** Step back in cached stream by 'count' bytes. Returns 'true' if cache contains resulting position. */
	bool rewind(size_t count)
	{
		if (mPos < count)
		{
			clear();
			return false;
		}
		else
		{
			mPos -= count;
			return true;
		}
	}

	/** Step forward in cached stream by 'count' bytes. Returns 'true' if cache contains resulting position. */
	bool ff(size_t count)
	{
		if (avail() < count)
		{
			clear();
			return false;
		}
		else
		{
			mPos += count;
			return true;
		}
	}

	/** Returns number of bytes available for reading in cache after rewinding. */
	size_t avail() const
	{
		return mValidBytes - mPos;
	}

	/** Clear the cache */
	void clear()
	{
		mValidBytes = 0;
		mPos = 0;
	}
};


class InStream : public Object, public GeneralAllocatedObject
{
public:
    InStream(const String& type, const String& name);
    InStream(const String& type, const String& name, va_list argp);
    virtual ~InStream();

    /** Reads up to {@code n} bytes from this stream and stores them in
        the byte array {@code s}.
		@param s Reference to a buffer pointer
		@param n Number of bytes to read
		@return The number of bytes actually read
    */
    virtual size_t read(u2byte* s, size_t n) = 0;

    /** Skips at most {@code count} bytes in this stream. The number of actual
        bytes skipped may be anywhere between 0 and {@code count}. If
        {@code count} is negative, this method does nothing and returns 0, but
        some subclasses may throw.
    
        <p>Note the "at most" in the description of this method: this method may
        choose to skip fewer bytes than requested. Callers should <i>always</i>
        check the return value.
        
		@param count Number of bytes to skip
        @return the number of bytes actually skipped.
    */
    virtual u2sszie_t skip(u2sszie_t count) = 0;

	/** Repositions the read point to a specified byte.
	*/
	virtual void seek(size_t pos) = 0;

	/** Returns the current byte offset from beginning. 
	*/
	virtual size_t tell(void) const = 0;

    virtual bool eof() const { return false; };

	/** Returns the total size of the data to be read from the stream,
	or 0 if this is indeterminate for this stream.
	*/
	size_t size(void) const { return m_uSize; }

    virtual void close() = 0;

protected:
	/// Size of the data in the stream (may be 0 if size cannot be determined)
	size_t			m_uSize;
};


typedef std::shared_ptr<InStream>		InStreamPtr;


class OutStream : public Object, public GeneralAllocatedObject
{
public:
	OutStream(const String& type, const String& name);
    OutStream(const String& type, const String& name, va_list argp);
    virtual ~OutStream();

    /** Writes {@code n} bytes from the byte array {@code s} to this stream.
    @param s the buffer to be written.
    @param n the number of bytes from {@code buffer} to write to this stream.
    */
    virtual size_t write(const u2byte* s, size_t n) = 0;

    /** Closes this stream. Implementations of this method should free any
        resources used by the stream. This implementation does nothing.
    */
    virtual void close() = 0;
};


typedef std::shared_ptr<OutStream>		OutStreamPtr;


U2EG_NAMESPACE_END

#endif /* defined(__U2InStream__) */
