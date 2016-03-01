//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Stream.h"

#include "U2MemoryAllocatorConfig.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
InStream::InStream(const String& type, const String& name)
: Object(type, name)
{
}
//-----------------------------------------------------------------------
InStream::InStream(const String& type, const String& name, va_list argp)
    : Object(type, name)
{

}
//-----------------------------------------------------------------------
InStream::~InStream()
{
}
//-----------------------------------------------------------------------
std::streamoff InStream::skip(std::streamoff count)
{
    u2byte* s = static_cast<u2byte*>(U2_MALLOC(count, MEMCATEGORY_GENERAL));
    u2int32 actually = read(s, count);
    U2_FREE(s, MEMCATEGORY_GENERAL);
    return actually;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
OutStream::OutStream(const String& type, const String& name)
    : Object(type, name)
{
}
//-----------------------------------------------------------------------
OutStream::OutStream(const String& type, const String& name, va_list argp)
    : Object(type, name)
{

}
//-----------------------------------------------------------------------
OutStream::~OutStream()
{
}