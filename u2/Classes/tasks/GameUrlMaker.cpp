#include "GameUrlMaker.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameUrlMaker::GameUrlMaker()
    : UrlMaker()
{
    m_szProtocol = "http";
    m_szAddress = "10.60.81.51";
    m_uPort = 8080;
}
//-----------------------------------------------------------------------
GameUrlMaker::~GameUrlMaker()
{

}
//-----------------------------------------------------------------------
inline void GameUrlMaker::setProtocol(const String& protocol)
{
    m_szProtocol = protocol;
}
//-----------------------------------------------------------------------
inline void GameUrlMaker::setAddress(const String& address)
{
    m_szAddress = address;
}
//-----------------------------------------------------------------------
inline void GameUrlMaker::setPort(u2uint32 port)
{
    m_uPort = port;
}