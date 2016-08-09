//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2DataFilterStream.h"

#include "U2MemoryAllocatorConfig.h"
#include "U2Stream.h"
#include "U2CodeCvt.h"


//---------------------------------------------------------------------
//---------------------------------------------------------------------
void flipEndian(void * pData, size_t size, size_t count)
{
    //#if U2_ENDIAN == U2_ENDIAN_BIG
    for (unsigned int index = 0; index < count; index++)
    {
        flipEndian((void *)((long)pData + (index * size)), size);
    }
    //#endif
}
//---------------------------------------------------------------------
void flipEndian(void * pData, size_t size)
{
    //#if U2_ENDIAN == U2_ENDIAN_BIG
    char swapByte;
    for (unsigned int byteIndex = 0; byteIndex < size / 2; byteIndex++)
    {
        swapByte = *(char *)((long)pData + byteIndex);
        *(char *)((long)pData + byteIndex) = *(char *)((long)pData + size - byteIndex - 1);
        *(char *)((long)pData + size - byteIndex - 1) = swapByte;
    }
    //#endif
}



U2EG_NAMESPACE_USING



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
DataFilterInStream::DataFilterInStream(const String& name, u2int32 endian)
: FilterInStream(GET_OBJECT_TYPE(DataFilterInStream), name)
, m_nEndian(endian)
{
}
//-----------------------------------------------------------------------
DataFilterInStream::DataFilterInStream(const String& name, va_list argp)
    : FilterInStream(GET_OBJECT_TYPE(DataFilterInStream), name)
    , m_nEndian(U2_ENDIAN_LITTLE)
{
    m_nEndian = va_arg(argp, u2int32);
}
//-----------------------------------------------------------------------
DataFilterInStream::~DataFilterInStream()
{
}
//-----------------------------------------------------------------------
void DataFilterInStream::_flipEndian(void * pData, size_t size, size_t count)
{
    if (m_nEndian != U2_ENDIAN)
    {
        flipEndian(pData, size, count);
    }
}
//-----------------------------------------------------------------------
bool DataFilterInStream::readBool()
{
    bool val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(bool));
    if (nResult < 0)
    {
        assert(0);
    }
    return val;
}
//-----------------------------------------------------------------------
u2int8 DataFilterInStream::readInt8()
{
    u2int8 val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(u2int8));
    if (nResult < 0)
    {
        assert(0);
    }
    return val;
}
//-----------------------------------------------------------------------
u2int16 DataFilterInStream::readInt16()
{
    u2int16 val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(u2int16));
    if (nResult < 0)
    {
        assert(0);
    }
    _flipEndian(&val, sizeof(u2int16), 1);
    return val;
}
//-----------------------------------------------------------------------
u2int32 DataFilterInStream::readInt32()
{
    u2int32 val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(u2int32));
    if (nResult < 0)
    {
        assert(0);
    }
    _flipEndian(&val, sizeof(u2int32), 1);
    return val;
}
//-----------------------------------------------------------------------
u2int64 DataFilterInStream::readInt64()
{
    u2int64 val = 0;
    u2int64 nResult = m_pIn->read((u2byte*)(&val), sizeof(u2int64));
    if (nResult < 0)
    {
        assert(0);
    }
    _flipEndian(&val, sizeof(u2int64), 1);
    return val;
}
//-----------------------------------------------------------------------
u2uint8 DataFilterInStream::readUint8()
{
    u2uint8 val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(u2uint8));
    if (nResult < 0)
    {
        assert(0);
    }
    return val;
}
//-----------------------------------------------------------------------
u2uint16 DataFilterInStream::readUint16()
{
    u2uint16 val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(u2uint16));
    if (nResult < 0)
    {
        assert(0);
    }
    _flipEndian(&val, sizeof(u2uint16), 1);
    return val;
}
//-----------------------------------------------------------------------
u2uint32 DataFilterInStream::readUint32()
{
    u2uint32 val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(u2uint32));
    if (nResult < 0)
    {
        assert(0);
    }
    _flipEndian(&val, sizeof(u2uint32), 1);
    return val;
}
//-----------------------------------------------------------------------
u2uint64 DataFilterInStream::readUint64()
{
    u2uint64 val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(u2uint64));
    if (nResult < 0)
    {
        assert(0);
    }
    _flipEndian(&val, sizeof(u2uint64), 1);
    return val;
}
//-----------------------------------------------------------------------
float DataFilterInStream::readFloat()
{
    float val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(float));
    if (nResult < 0)
    {
        assert(0);
    }
    return val;
}
//-----------------------------------------------------------------------
double DataFilterInStream::readDouble()
{
    double val = 0;
    u2int32 nResult = m_pIn->read((u2byte*)(&val), sizeof(double));
    if (nResult < 0)
    {
        assert(0);
    }
    return val;
}
//-----------------------------------------------------------------------
// u2int32 DataInStream::readUTFChar(u2char* utf)
// {
//     utf[0] = readInt8();
//     u2int32 nLen = CodeCvt::utf8_charlen((const char*)utf);
//     u2int32 i = 1;
//     for (; i < nLen; i++)
//     {
//         utf[i] = readInt8();
//     }
//     return i;
// }
//-----------------------------------------------------------------------
u2::String DataFilterInStream::readUTFString()
{
    u2uint32 usLen = readUint16();

#if U2_WCHAR_T_STRINGS
    u2::String str;

    u2char* c = static_cast<u2char*>(U2_MALLOC(6, MEMCATEGORY_GENERAL));
    for (u2uint32 i = 0; i < usLen; i++)
    {
        memset(c, 0, 6);

        c[0] = readInt8();
        u2int32 nByteCount = CodeCvt::utf8_charlen((const char*)&c);
        for (u2int32 j = 1; j < nByteCount; j++)
        {
            c[j] = readInt8();
        }

        u2::Char wc;
        CodeCvt::mbstowcs(&wc, (const char*)c, 1);
        str += wc;
    }
    U2_FREE(c, MEMCATEGORY_GENERAL);

    return str;
#else
//     u2::String str;
//     for (u2uint32 i = 0; i < usLen; i++)
//     {
//         u2char c = (u2char)readInt8();
//         str += c;
//         u2int32 nByteCount = CodeCvt::utf8_charlen((const char*)&c);
//         for (u2int32 j = 1; j < nByteCount; j++)
//         {
//             str += readInt8();
//         }
//     }
//     return str;
    u2::String str;
    for (u2uint32 i = 0; i < usLen; i++)
    {
        str += (u2char)readInt8();
    }
    return str;
#endif

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
DataFilterOutStream::DataFilterOutStream(const String& name, u2int32 endian)
    : FilterOutStream(GET_OBJECT_TYPE(DataFilterOutStream), name)
    , m_nEndian(endian)
{
}
//-----------------------------------------------------------------------
DataFilterOutStream::DataFilterOutStream(const String& name, va_list argp)
    : FilterOutStream(GET_OBJECT_TYPE(DataFilterOutStream), name)
    , m_nEndian(U2_ENDIAN_LITTLE)
{
    m_nEndian = va_arg(argp, u2int32);
}
//-----------------------------------------------------------------------
DataFilterOutStream::~DataFilterOutStream()
{
}
//-----------------------------------------------------------------------
void DataFilterOutStream::_flipEndian(void * pData, size_t size, size_t count)
{
    if (m_nEndian != U2_ENDIAN)
    {
        flipEndian(pData, size, count);
    }
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeBool(bool val)
{
    m_pOut->write((const u2byte*)&val, sizeof(bool));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeInt8(u2int8 val)
{
    m_pOut->write((const u2byte*)&val, sizeof(u2int8));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeInt16(u2int16 val)
{
    _flipEndian(&val, sizeof(u2int16), 1);
    m_pOut->write((const u2byte*)&val, sizeof(u2int16));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeInt32(u2int32 val)
{
    _flipEndian(&val, sizeof(u2int32), 1);
    m_pOut->write((const u2byte*)&val, sizeof(u2int32));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeInt64(u2int64 val)
{
    _flipEndian(&val, sizeof(u2int64), 1);
    m_pOut->write((const u2byte*)&val, sizeof(u2int64));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeUint8(u2uint8 val)
{
    m_pOut->write((const u2byte*)&val, sizeof(u2uint8));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeUint16(u2uint16 val)
{
    _flipEndian(&val, sizeof(u2uint16), 1);
    m_pOut->write((const u2byte*)&val, sizeof(u2uint16));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeUint32(u2uint32 val)
{
    _flipEndian(&val, sizeof(u2uint32), 1);
    m_pOut->write((const u2byte*)&val, sizeof(u2uint32));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeUint64(u2uint64 val)
{
    _flipEndian(&val, sizeof(u2uint64), 1);
    m_pOut->write((const u2byte*)&val, sizeof(u2uint64));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeFloat(float val)
{
    m_pOut->write((const u2byte*)&val, sizeof(float));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeDouble(double val)
{
    m_pOut->write((const u2byte*)&val, sizeof(double));
}
//-----------------------------------------------------------------------
void DataFilterOutStream::writeUTFString(u2::String val)
{
#if U2_WCHAR_T_STRINGS
    u2::u2char* c = static_cast<u2::u2char*>(U2_MALLOC(6 * val.size(), MEMCATEGORY_GENERAL));
    int len = CodeCvt::wcstombs((char*)c, val.c_str(), 6 * val.size());
    writeUint16(val.size());
    m_pOut->write((const u2byte*)c, len);
    U2_FREE(c, MEMCATEGORY_GENERAL);
#else
    size_t len = val.size();
    writeUint16(len);
    m_pOut->write((const u2byte*)(val.c_str()), len);
#endif
}