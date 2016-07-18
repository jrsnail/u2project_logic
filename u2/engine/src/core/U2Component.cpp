#include "U2Component.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Component::Component(const String& type, const String& name)
    : Object(type, name)
{
}
//-----------------------------------------------------------------------
Component::~Component()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ComponentManager* Singleton<ComponentManager>::msSingleton = 0;
ComponentManager* ComponentManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new ComponentManager;
    }
    return msSingleton;
}
ComponentManager& ComponentManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ComponentManager::ComponentManager()
{
}
//-----------------------------------------------------------------------
ComponentManager::~ComponentManager()
{
}