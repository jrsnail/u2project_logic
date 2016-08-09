#include "U2System.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
System::System(const String& type, const String& name, const String& guid)
    : Object(type, name, guid)
    , m_uPriority(0)
{
}
//-----------------------------------------------------------------------
System::~System()
{
}
//-----------------------------------------------------------------------
void System::initialize(size_t priority)
{
    m_uPriority = priority;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> SystemManager* Singleton<SystemManager>::msSingleton = 0;
SystemManager* SystemManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new SystemManager;
    }
    return msSingleton;
}
SystemManager& SystemManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
SystemManager::SystemManager()
{
}
//-----------------------------------------------------------------------
SystemManager::~SystemManager()
{
}
//-----------------------------------------------------------------------
System* SystemManager::createObject(const String& type, const String& name, const String& guid, size_t priority)
{
    System* pObj = createObject(type, name, guid);
    if (pObj == nullptr)
    {
        assert(0);
    }
    else
    {
        pObj->initialize(priority);

        PriorityMap::iterator it = m_ActiveMap.find(priority);
        if (it == m_ActiveMap.end())
        {
            m_ActiveMap[priority] = pObj;
        }
        else
        {
            assert(0);
        }
    }
    return pObj;
}
//-----------------------------------------------------------------------
System* SystemManager::createObject(const String& type, const String& name, const String& guid)
{
    return SimpleObjectManager<System>::createObject(type, name, guid);
}
//-----------------------------------------------------------------------
void SystemManager::destoryObject(System* obj)
{
    assert(obj != nullptr);

    size_t priority = obj->getPriority();
    PriorityMap::iterator it = m_ActiveMap.find(priority);
    if (it == m_ActiveMap.end())
    {
        assert(0);
    }
    else
    {
        m_ActiveMap.erase(it);
    }

    SimpleObjectManager<System>::destoryObject(obj);
}
//-----------------------------------------------------------------------
void SystemManager::enter()
{
    for (PriorityMap::iterator it = m_ActiveMap.begin(); it != m_ActiveMap.end(); it++)
    {
        System* pSystem = it->second;
        if (pSystem != nullptr)
        {
            pSystem->enter();
        }
    }
}
//-----------------------------------------------------------------------
void SystemManager::execute(u2real dt)
{
    for (PriorityMap::iterator it = m_ActiveMap.begin(); it != m_ActiveMap.end(); it++)
    {
        System* pSystem = it->second;
        if (pSystem != nullptr)
        {
            pSystem->execute(dt);
        }
    }
}
//-----------------------------------------------------------------------
void SystemManager::exit()
{
    for (PriorityMap::iterator it = m_ActiveMap.begin(); it != m_ActiveMap.end(); it++)
    {
        System* pSystem = it->second;
        if (pSystem != nullptr)
        {
            pSystem->exit();
        }
    }
}
//-----------------------------------------------------------------------
void SystemManager::pause()
{
    for (PriorityMap::iterator it = m_ActiveMap.begin(); it != m_ActiveMap.end(); it++)
    {
        System* pSystem = it->second;
        if (pSystem != nullptr)
        {
            pSystem->pause();
        }
    }
}
//-----------------------------------------------------------------------
void SystemManager::resume()
{
    for (PriorityMap::iterator it = m_ActiveMap.begin(); it != m_ActiveMap.end(); it++)
    {
        System* pSystem = it->second;
        if (pSystem != nullptr)
        {
            pSystem->resume();
        }
    }
}