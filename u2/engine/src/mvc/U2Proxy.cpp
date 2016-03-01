//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Proxy.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Proxy::Proxy(const String& type, const String& name, void const* data)
    : Object(type, name)
    , _data(data)
{ }
//-----------------------------------------------------------------------
Proxy::~Proxy(void)
{ }
//-----------------------------------------------------------------------
inline void Proxy::setData(void const* data)
{
    _data = data;
}
//-----------------------------------------------------------------------
inline void const* Proxy::getData(void) const
{
    return _data;
}
//-----------------------------------------------------------------------
inline void Proxy::onRegister(void)
{ }
//-----------------------------------------------------------------------
inline void Proxy::onRemove(void)
{ }
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ProxyManager* Singleton<ProxyManager>::msSingleton = 0;
ProxyManager* ProxyManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new ProxyManager;
    }
    return msSingleton;
}
ProxyManager& ProxyManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ProxyManager::ProxyManager()
{
}
//-----------------------------------------------------------------------
ProxyManager::~ProxyManager()
{
}