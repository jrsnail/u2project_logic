#include "U2System.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
System::System(const String& type, const String& name)
    : Object(type, name)
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
void SystemManager::enter()
{
    for (PriorityMap::iterator it = m_ActiveMap.begin(); it != m_ActiveMap.end(); it++)
    {
        System* pSystem = it->second;
        if (pSystem == nullptr)
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
        if (pSystem == nullptr)
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
        if (pSystem == nullptr)
        {
            pSystem->exit();
        }
    }
}