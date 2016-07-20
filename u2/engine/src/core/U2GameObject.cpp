#include "U2GameObject.h"

#include "U2Component.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameObject::GameObject(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Resource(creator, type, handle, group, isManual, loader)
    , Prototype(type, name)
{
}
//-----------------------------------------------------------------------
GameObject::~GameObject()
{
}
//-----------------------------------------------------------------------
void GameObject::copy(const GameObject& src)
{
    
}
//-----------------------------------------------------------------------
GameObject* GameObject::cloneFromPrototype(const String& name)
{
    GameObject* pPrototype = retrievePrototype();
    GameObject* pNewGameObj = GameObjectManager::getSingleton().createObject(pPrototype->getType(), name);
    pNewGameObj->copy(*pPrototype);
    pPrototype->addInstance(pNewGameObj);
    return pNewGameObj;
}
//-----------------------------------------------------------------------
GameObject* GameObject::cloneFromInstance(const String& name)
{
    GameObject* pNewGameObj = GameObjectManager::getSingleton().createObject(this->getType(), name);
    pNewGameObj->copy(*this);
    GameObject* pPrototype = retrievePrototype();
    pPrototype->addInstance(pNewGameObj);
    return pNewGameObj;
}
//-----------------------------------------------------------------------
void GameObject::resetFromPrototype()
{
    if (m_pPrototype == nullptr)
    {
        return;
    }
    copy(*m_pPrototype);
}
//-----------------------------------------------------------------------
void GameObject::applyToPrototype()
{
    if (m_pPrototype == nullptr)
    {
        return;
    }
    m_pPrototype->copy(*this);
}
//-----------------------------------------------------------------------
void GameObject::loadImpl(void)
{

}
//-----------------------------------------------------------------------
void GameObject::unloadImpl(void)
{

}
//-----------------------------------------------------------------------
u2::Component* GameObject::createComponent(const String& type, const String& name)
{
    u2::Component* pComp = ComponentManager::getSingleton().createObject(type, name);
    pComp->bornOn(this);
    return pComp;
}
//-----------------------------------------------------------------------
void GameObject::destroyComponent(u2::Component* comp)
{
    assert(comp);
    if (comp->getBornGameObject() == this)
    {
        removeComponent(comp);
        ComponentManager::getSingleton().destoryObject(comp);
    }
}
//-----------------------------------------------------------------------
void GameObject::addComponent(u2::Component* comp)
{
    assert(comp);

    m_ComponentMap.insert(std::make_pair(comp->getType(), comp));
}
//-----------------------------------------------------------------------
void GameObject::removeComponent(u2::Component* comp)
{
    assert(comp);
    ComponentPair p = m_ComponentMap.equal_range(comp->getType());
    for (TypedComponentMap::iterator it = p.first; it != p.second; it++)
    {
        if (comp == it->second)
        {
            m_ComponentMap.erase(it);
            return;
        }
    }
}
//-----------------------------------------------------------------------
u2::Component* GameObject::retrieveComponentByGuid(const String& guid)
{
    for (TypedComponentMap::iterator it = m_ComponentMap.begin(); it != m_ComponentMap.end(); it++)
    {
        u2::Component* pObj = it->second;
        if (pObj->getGuid() == guid)
        {
            return pObj;
        }
    }
    return nullptr;
}
//-----------------------------------------------------------------------
GameObject* GameObject::createGameObject(const String& type, const String& name)
{
    ResourcePtr resPtr = GameObjectManager::getSingleton().getResourceByName(type);
    if (resPtr)
    {
        GameObject* pGameObj = dynamic_cast<GameObject*>(resPtr.get());
        if (pGameObj)
        {
            return pGameObj->cloneFromPrototype(name);
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        assert(0);
    }
    return nullptr;
}
//-----------------------------------------------------------------------
void GameObject::destroyGameObject(GameObject* gameObj)
{
    assert(gameObj);
    if (gameObj->isPrototype())
    {
        // In editor, we should delete prototype GameObject actually, 
        // but now in game, we just assert it.
        assert(0);
    }
    else
    {
        removeGameObject(gameObj);
        GameObjectManager::getSingleton().destoryObject(gameObj);
    }
}
//-----------------------------------------------------------------------
void GameObject::addGameObject(GameObject* gameObj)
{
    assert(gameObj);

    m_GameObjMap.insert(std::make_pair(gameObj->getType(), gameObj));
}
//-----------------------------------------------------------------------
void GameObject::removeGameObject(GameObject* gameObj)
{
    assert(gameObj);
    GameObjectPair p = m_GameObjMap.equal_range(gameObj->getType());
    for (TypedGameObjectMap::iterator it = p.first; it != p.second; it++)
    {
        if (gameObj == it->second)
        {
            m_GameObjMap.erase(it);
            return;
        }
    }
    assert(0);
}
//-----------------------------------------------------------------------
GameObject* GameObject::retrieveGameObjectByGuid(const String& guid)
{
    for (TypedGameObjectMap::iterator it = m_GameObjMap.begin(); it != m_GameObjMap.end(); it++)
    {
        GameObject* pObj = it->second;
        if (pObj->getGuid() == guid)
        {
            return pObj;
        }
    }
    return nullptr;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> GameObjectManager* Singleton<GameObjectManager>::msSingleton = 0;
GameObjectManager* GameObjectManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new GameObjectManager;
    }
    return msSingleton;
}
GameObjectManager& GameObjectManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
GameObjectManager::GameObjectManager()
{
    // Loading order
    mLoadOrder = 1000.0f;
}
//-----------------------------------------------------------------------
GameObjectManager::~GameObjectManager()
{
}
//-----------------------------------------------------------------------
void GameObjectManager::parseScript(InStreamPtr& stream, const String& groupName)
{

}
//-----------------------------------------------------------------------
Resource* GameObjectManager::createImpl(const String& name, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader,
    const NameValuePairList* createParams)
{
    return U2_NEW GameObject(this, name, handle, group
        , String("prototype_gameobject_") + name, isManual, loader);
}
//-----------------------------------------------------------------------
GameObject* GameObjectManager::createObject(const String& type, const String& name)
{
    return m_InstanceCollection.createObject(type, name);
}
//-----------------------------------------------------------------------
void GameObjectManager::destoryObject(GameObject* obj)
{
    m_InstanceCollection.destoryObject(obj);
}
//-----------------------------------------------------------------------
GameObject* GameObjectManager::retrieveObjectByTN(const String& type, const String& name)
{
    return m_InstanceCollection.retrieveObjectByTN(type, name);
}
//-----------------------------------------------------------------------
TypedObjectManager<GameObject>::ObjectMapIterator GameObjectManager::retrieveAllObjectsByType(const String& type)
{
    return m_InstanceCollection.retrieveAllObjectsByType(type);
}
//-----------------------------------------------------------------------
GameObject* GameObjectManager::retrieveObjectByGuid(const String& guid)
{
    return m_InstanceCollection.retrieveObjectByGuid(guid);
}
//-----------------------------------------------------------------------
GameObject* GameObjectManager::retrieveObjectByType(const String& type)
{
    return m_InstanceCollection.retrieveObjectByType(type);
}