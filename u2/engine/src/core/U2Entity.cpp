#include "U2Entity.h"

#include "U2Component.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Entity::Entity(const String& type, const String& name)
    : Object(type, name)
{
}
//-----------------------------------------------------------------------
Entity::~Entity()
{
}
//-----------------------------------------------------------------------
void Entity::addComponent(Component* comp)
{
    assert(comp);
    m_TypedMap.insert(std::make_pair(comp->getType(), comp));
}
//-----------------------------------------------------------------------
void Entity::removeComponent(Component* comp)
{
    assert(comp);
    Pair p = m_TypedMap.equal_range(comp->getType());
    for (TypedComponentMap::iterator it = p.first; it != p.second; it++)
    {
        if (comp == it->second)
        {
            m_TypedMap.erase(it);
            return;
        }
    }
}
//-----------------------------------------------------------------------
Component* Entity::retrieveComponentByGuid(const String& guid)
{
    for (TypedComponentMap::iterator it = m_TypedMap.begin(); it != m_TypedMap.end(); it++)
    {
        Component* pObj = it->second;
        if (pObj->getGuid() == guid)
        {
            return pObj;
        }
    }
    return nullptr;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> EntityManager* Singleton<EntityManager>::msSingleton = 0;
EntityManager* EntityManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new EntityManager;
    }
    return msSingleton;
}
EntityManager& EntityManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
EntityManager::EntityManager()
{
}
//-----------------------------------------------------------------------
EntityManager::~EntityManager()
{
}