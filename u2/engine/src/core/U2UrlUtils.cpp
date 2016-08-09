#include "U2UrlUtils.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
u2uchar UrlEncoder::toHex(u2uchar x)
{
    return  x > 9 ? x + 55 : x + 48;
}
//-----------------------------------------------------------------------
u2uchar UrlEncoder::fromHex(u2uchar x)
{
    u2uchar y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}
//-----------------------------------------------------------------------
String UrlEncoder::encode(const String& str)
{
    String strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (isalnum((u2uchar)str[i]) 
            || (str[i] == '-') 
            || (str[i] == '_') 
            || (str[i] == '.') 
            || (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else
        {
            strTemp += '%';
            strTemp += toHex((u2uchar)str[i] >> 4);
            strTemp += toHex((u2uchar)str[i] % 16);
        }
    }
    return strTemp;
}
//-----------------------------------------------------------------------
String UrlEncoder::decode(const String& str)
{
    String strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%')
        {
            assert(i + 2 < length);
            u2uchar high = fromHex((u2uchar)str[++i]);
            u2uchar low = fromHex((u2uchar)str[++i]);
            strTemp += high * 16 + low;
        }
        else strTemp += str[i];
    }
    return strTemp;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
UrlMaker::UrlMaker()
    : m_uPort(80)
{

}
//-----------------------------------------------------------------------
UrlMaker::~UrlMaker()
{

}
//-----------------------------------------------------------------------
inline void UrlMaker::setProtocol(const String& protocol)
{
    m_szProtocol = protocol;
}
//-----------------------------------------------------------------------
inline void UrlMaker::setAddress(const String& address)
{
    m_szAddress = address;
}
//-----------------------------------------------------------------------
inline void UrlMaker::setPort(u2uint32 port)
{
    m_uPort = port;
}
//-----------------------------------------------------------------------
inline void UrlMaker::setPath(const String& path)
{
    m_szPath = path;
}
//-----------------------------------------------------------------------
void UrlMaker::addParam(const String& key, const String& value)
{
    ParamList::iterator it = m_ParamList.find(key);
    if (it == m_ParamList.end())
    {
        m_ParamList.insert(make_pair(key, value));
    }
    else
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
String UrlMaker::generate()
{
    StringStream ss;
    //ss << "http://10.60.81.51:8080/plane/operation?";
    ss << m_szProtocol;
    ss << "://" << m_szAddress;
    ss << ":" << m_uPort;
    ss << "/" << m_szPath;

    for (ParamList::iterator it = m_ParamList.begin(); 
    it != m_ParamList.end(); it++)
    {
        if (it == m_ParamList.begin())
        {
            ss << "?";
        }
        else
        {
            ss << "&";
        }
        ss << UrlEncoder::encode(it->first) << "=" << UrlEncoder::encode(it->second);
    }

    return ss.str();
}