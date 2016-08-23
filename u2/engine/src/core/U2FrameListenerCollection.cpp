#include "U2FrameListenerCollection.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> FrameListenerCollection* Singleton<FrameListenerCollection>::msSingleton = 0;
//-----------------------------------------------------------------------
FrameListenerCollection* FrameListenerCollection::getSingletonPtr(void)
{
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
    // copy, avaid to interrupt iterator
    std::vector<Func> vec;
    for (FuncMap::const_iterator it = m_funcMap.begin();
    it != m_funcMap.end(); it++)
    {
        vec.push_back(it->second);
    }

    // execute callback function
    for (std::vector<Func>::const_iterator it = vec.begin();
    it != vec.end(); it++)
    {
        (*it)(dt);
    }
}