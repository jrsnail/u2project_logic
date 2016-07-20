#include "U2Component.h"

#include "U2GameObject.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Component::Component(const String& type, const String& name)
    : Object(type, name)
    , m_pBornGameObj(nullptr)
    , m_pAttachedGameObj(nullptr)
    , m_uState(CS_None)
{
}
//-----------------------------------------------------------------------
Component::~Component()
{
}
//-----------------------------------------------------------------------
void Component::bornOn(GameObject* gameObj)
{
    assert(m_pBornGameObj == nullptr);
    m_pBornGameObj = gameObj;
}
//-----------------------------------------------------------------------
GameObject* Component::getBornGameObject() const
{
    return m_pBornGameObj;
}
//-----------------------------------------------------------------------
void Component::attachedOn(GameObject* gameObj)
{
    assert(m_pAttachedGameObj == nullptr);
    m_pAttachedGameObj = gameObj;
}
//-----------------------------------------------------------------------
GameObject* Component::getAttachedGameObject() const
{
    return m_pAttachedGameObj;
}
//-----------------------------------------------------------------------
void Component::setState(size_t state)
{
    m_uState = state;
}
//-----------------------------------------------------------------------
size_t Component::getState() const
{
    return m_uState;
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