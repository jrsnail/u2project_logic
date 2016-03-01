//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2DataStream__
#define __U2DataStream__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2FilterStream.h"



static void flipEndian(void * pData, size_t size, size_t count);
static void flipEndian(void * pData, size_t size);



U2EG_NAMESPACE_BEGIN


class DataFilterInStream : public FilterInStream
{
public:
    DataFilterInStream(const String& name, u2int32 endian = U2_ENDIAN_LITTLE);
    DataFilterInStream(const String& name, va_list argp);
    virtual ~DataFilterInStream();

    virtual bool readBool();

    virtual u2int8 readInt8();
    virtual u2int16 readInt16();
    virtual u2int32 readInt32();
    virtual u2int64 readInt64();

    virtual u2uint8 readUint8();
    virtual u2uint16 readUint16();
    virtual u2uint32 readUint32();
    virtual u2uint64 readUint64();

    virtual float readFloat();
    virtual double readDouble();

    //virtual u2int32 readUTFChar(u2char* utf);
    virtual u2::String readUTFString();

protected:
    void _flipEndian(void * pData, size_t size, size_t count);
    
protected:
    u2int32     m_nEndian;
};


class DataFilterOutStream : public FilterOutStream
{
public:
    DataFilterOutStream(const String& name, u2int32 endian = U2_ENDIAN_LITTLE);
    DataFilterOutStream(const String& name, va_list argp);
    virtual ~DataFilterOutStream();

    virtual void writeBool(bool val);

    virtual void writeInt8(u2int8 val);
    virtual void writeInt16(u2int16 val);
    virtual void writeInt32(u2int32 val);
    virtual void writeInt64(u2int64 val);

    virtual void writeUint8(u2uint8 val);
    virtual void writeUint16(u2uint16 val);
    virtual void writeUint32(u2uint32 val);
    virtual void writeUint64(u2uint64 val);

    virtual void writeFloat(float val);
    virtual void writeDouble(double val);

    //virtual u2int32 readUTFChar(u2char* utf);
    virtual void writeUTFString(u2::String val);

protected:
    void _flipEndian(void * pData, size_t size, size_t count);

protected:
    u2int32     m_nEndian;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2FilterStream__) */
