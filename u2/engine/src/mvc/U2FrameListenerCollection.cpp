//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2FrameListenerCollection.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> FrameListenerCollection* Singleton<FrameListenerCollection>::msSingleton = 0;
//-----------------------------------------------------------------------
FrameListenerCollection* FrameListenerCollection::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        assert(0);
    }
    return msSingleton;
}
//-----------------------------------------------------------------------
FrameListenerCollection& FrameListenerCollection::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
FrameListenerCollection::FrameListenerCollection()
{
}
//-----------------------------------------------------------------------
FrameListenerCollection::~FrameListenerCollection()
{
}
//-----------------------------------------------------------------------
void FrameListenerCollection::addFrameListener(void* address, Func fun)
{
    FuncMap::iterator it = m_funcMap.find(address);
    if (it != m_funcMap.end())
    {
        assert(0);
    }
    else
    {
        m_funcMap.insert(make_pair(address, fun));
    }
}
//-----------------------------------------------------------------------
void FrameListenerCollection::removeFrameListener(void* address)
{
    FuncMap::iterator it = m_funcMap.find(address);
    if (it != m_funcMap.end())
    {
        m_funcMap.erase(it);
    }
}
//-----------------------------------------------------------------------
void FrameListenerCollection::_onUpdate(float dt)
{
    while (m_funcMap.size() > 0)
    {
        FuncMap::const_iterator it = m_funcMap.begin();
        (it->second)(dt);
    }
}