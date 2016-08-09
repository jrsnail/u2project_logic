#ifndef __U2UrlUtils_H__
#define __U2UrlUtils_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class UrlEncoder
{
public:
    static u2uchar toHex(u2uchar x);

    static u2uchar fromHex(u2uchar x);

    static String encode(const String& str);

    static String decode(const String& str);
};


class UrlMaker
{
public:
    UrlMaker();
    virtual ~UrlMaker();

    virtual void setProtocol(const String& protocol);
    virtual void setAddress(const String& address);
    virtual void setPort(u2uint32 port);
    virtual void setPath(const String& path);
    virtual void addParam(const String& key, const String& value);

    String generate();

protected:
    String      m_szProtocol;
    String      m_szAddress;
    String      m_szPath;
    u2uint32    m_uPort;

    typedef map<String, String>::type   ParamList;
    ParamList   m_ParamList;
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2UrlUtils_H__
