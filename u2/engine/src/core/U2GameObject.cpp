#include "U2GameObject.h"

#include "U2Component.h"
#include "U2XmlSerialize.h"
#include "tinyxml.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameObject::GameObject(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Resource(creator, type, handle, group, isManual, loader)
    , Prototype(type, String("prototype_gameobject_") + type)
    , m_pParentGameObj(nullptr)
{
}
//-----------------------------------------------------------------------
GameObject::GameObject(const String& type, const String& name)
    : Resource()
    , Prototype(type, name)
    , m_pParentGameObj(nullptr)
{

}
//-----------------------------------------------------------------------
GameObject::~GameObject()
{
    GameObject* pPrototype = this->retrievePrototype();
    if (pPrototype)
    {
        pPrototype->removeInstance(this);
    }
}
//-----------------------------------------------------------------------
void GameObject::copy(const GameObject& src)
{
    
}
//-----------------------------------------------------------------------
GameObject* GameObject::cloneFromPrototype(const String& name)
{
    GameObject* pPrototype = retrievePrototype();
    GameObject* pNewGameObj = GameObjectManager::getSingleton()._createObject(pPrototype->getType(), name);
    pNewGameObj->copy(*pPrototype);
    pPrototype->addInstance(pNewGameObj);
    return pNewGameObj;
}
//-----------------------------------------------------------------------
GameObject* GameObject::cloneFromInstance(const String& name)
{
    GameObject* pNewGameObj = GameObjectManager::getSingleton()._createObject(this->getType(), name);
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
    comp->attachedOn(this);

    // listeners
    for (ListenerList::iterator it = m_Listeners.begin();
    it != m_Listeners.end(); it++)
    {
        (*it)->onAttachComponent(this, comp);
    }
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

            // listeners
            for (ListenerList::iterator it = m_Listeners.begin();
            it != m_Listeners.end(); it++)
            {
                (*it)->onDetachComponent(this, comp);
            }

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
GameObject* GameObject::createChildGameObject(const String& type, const String& name)
{
    GameObject* pObj = GameObjectManager::getSingleton().createObject(type, name);
    if (pObj)
    {
        addChildGameObject(pObj);
    }
    else
    {
        assert(0);
    }
    return nullptr;
}
//-----------------------------------------------------------------------
void GameObject::destroyChildGameObject(GameObject* gameObj)
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
        removeChildGameObject(gameObj);
        GameObjectManager::getSingleton().destoryObject(gameObj);
    }
}
//-----------------------------------------------------------------------
void GameObject::addChildGameObject(GameObject* gameObj)
{
    assert(gameObj);

    m_GameObjMap.insert(std::make_pair(gameObj->getType(), gameObj));
    gameObj->m_pParentGameObj = this;
}
//-----------------------------------------------------------------------
void GameObject::removeChildGameObject(GameObject* gameObj)
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
GameObject* GameObject::retrieveChildGameObjectByGuid(const String& guid)
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
//---------------------------------------------------------------------
GameObject* GameObject::retrieveParentGameObject()
{
    return m_pParentGameObj;
}
//---------------------------------------------------------------------
void GameObject::addListener(Listener* listener)
{
    ListenerList::iterator it
        = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
    if (it != m_Listeners.end())
    {
        m_Listeners.push_back(listener);
    }
}
//---------------------------------------------------------------------
void GameObject::removeListener(Listener* listener)
{
    ListenerList::iterator it
        = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
    if (it != m_Listeners.end())
    {
        m_Listeners.erase(it);
    }
}
//---------------------------------------------------------------------
bool GameObject::isPrototypeDependent()
{
    if (m_pParentGameObj != nullptr)
    {
        if (m_pParentGameObj->isPrototype())
        {
            return true;
        }
        else
        {
            return m_pParentGameObj->isPrototypeDependent();
        }
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------
bool GameObject::_loadFromXml(const TiXmlElement* gameObjElem, String& error)
{
    const TiXmlElement* pGameObjElem = gameObjElem;
    String& szError = error;

    do
    {
        const char* pszGameObjType = pGameObjElem->Attribute("type");
        GET_ERROR_LINE_AND_BREAK(pszGameObjType, szError);
        const char* pszGameObjName = pGameObjElem->Attribute("name");
        GET_ERROR_LINE_AND_BREAK(pszGameObjName, szError);

        if (isPrototype())
        {
            const char* pszGameObjGuid = pGameObjElem->Attribute("guid");
            GET_ERROR_LINE_AND_BREAK(pszGameObjGuid, szError);
        }
        else
        {

        }

        // parse every component
        for (const TiXmlElement* pCompElem = pGameObjElem->FirstChildElement("Component");
        pCompElem; pCompElem = pCompElem->NextSiblingElement("Component"))
        {
            const char* pszCompType = pCompElem->Attribute("type");
            GET_ERROR_LINE_AND_BREAK(pszCompType, szError);
            const char* pszCompName = pCompElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszCompName, szError);
            Component* pComp = ComponentManager::getSingleton().createObject(pszCompType, pszCompName);
            if (!pComp->_loadFromXml(pCompElem, szError))
            {
                ComponentManager::getSingleton().destoryObject(pComp);
                break;
            }
            else
            {
                this->addComponent(pComp);
            }
        }

        // parse every children game object
        for (const TiXmlElement* pChildGameObjElem = pGameObjElem->FirstChildElement("GameObject");
        pChildGameObjElem; pChildGameObjElem = pChildGameObjElem->NextSiblingElement("GameObject"))
        {
            const char* pszGameObjType = pChildGameObjElem->Attribute("type");
            GET_ERROR_LINE_AND_BREAK(pszGameObjType, szError);
            const char* pszGameObjName = pChildGameObjElem->Attribute("name");
            GET_ERROR_LINE_AND_BREAK(pszGameObjName, szError);

            GameObject* pChildGameObj
                = GameObjectManager::getSingleton().createObject(pszGameObjType, pszGameObjName);
            if (!pChildGameObj->_loadFromXml(pChildGameObjElem, szError))
            {
                break;
            }
            else
            {
                this->addChildGameObject(pChildGameObj);
            }
        }

    } while (0);

    return szError == BLANK;
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

    // Scripting is supported by this manager
    mScriptPatterns.push_back("*.gobj");
    ResourceGroupManager::getSingleton()._registerScriptLoader(this);

    // Resource type
    mResourceType = "GameObject";

    // Register with resource group manager
    ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}
//-----------------------------------------------------------------------
GameObjectManager::~GameObjectManager()
{
    // Unregister with resource group manager
    ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}
//-----------------------------------------------------------------------
void GameObjectManager::parseScript(InStreamPtr& stream, const String& groupName)
{
    XmlSerializeManager::getSingleton().load(stream, groupName);
}
//-----------------------------------------------------------------------
Resource* GameObjectManager::createImpl(const String& name, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader,
    const NameValuePairList* createParams)
{
    return U2_NEW GameObject(this, name, handle, group, isManual, loader);
}
//-----------------------------------------------------------------------
GameObjectPtr GameObjectManager::create(const String& name, const String& group,
    bool isManual, ManualResourceLoader* loader, const NameValuePairList* createParams)
{
    return std::dynamic_pointer_cast<GameObject>(
        createResource(name, group, isManual, loader, createParams));
}
//-----------------------------------------------------------------------
GameObject* GameObjectManager::createObject(const String& type, const String& name)
{
    ResourcePtr resPtr = this->getResourceByName(type);
    // prototype existed
    if (resPtr)
    {
        GameObject* pPrototype = dynamic_cast<GameObject*>(resPtr.get());
        // create instance from prototype
        if (pPrototype)
        {
            GameObject* pObj = pPrototype->cloneFromPrototype(name);
            return pObj;
        }
        else
        {
            assert(0);
        }
    }
    // prototype not existed
    else
    {
        // create as an alone object
        CREATE_FACTORY_WITH_TYPE(GameObject, type);
        GameObject* pObj = GameObjectManager::getSingleton()._createObject(type, name);
        return pObj;
    }
    return nullptr;
}
//-----------------------------------------------------------------------
void GameObjectManager::destoryObject(GameObject* obj)
{
    assert(obj);
    if (obj->isPrototype() || obj->isPrototypeDependent())
    {
        // In editor, we should delete prototype GameObject actually, 
        // but now in game, we just assert it.
        assert(0);
    }
    else
    {
        m_InstanceCollection.destoryObject(obj);
    }
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
//-----------------------------------------------------------------------
GameObject* GameObjectManager::_createObject(const String& type, const String& name)
{
    return m_InstanceCollection.createObject(type, name);
}
//-----------------------------------------------------------------------
void GameObjectManager::onAttachComponent(GameObject* gameObj, Component* comp)
{
    assert(gameObj != nullptr);
    assert(comp != nullptr);
    const String& szCompType = comp->getType();
    CompRefPair p = m_CompRefMap.equal_range(szCompType);
    for (CompRefMap::iterator it = p.first; it != p.second; it++)
    {
        if (it->second.pGameObj == gameObj)
        {
            it->second.uRefCount++;
            return;
        }
    }
    m_CompRefMap.insert(make_pair(szCompType, StGameObjRef(gameObj, 1)));
}
//-----------------------------------------------------------------------
void GameObjectManager::onDetachComponent(GameObject* gameObj, Component* comp)
{
    assert(gameObj != nullptr);
    assert(comp != nullptr);
    const String& szCompType = comp->getType();
    CompRefPair p = m_CompRefMap.equal_range(szCompType);
    for (CompRefMap::iterator it = p.first; it != p.second; it++)
    {
        if (it->second.pGameObj == gameObj)
        {
            it->second.uRefCount--;
            if (it->second.uRefCount == 0)
            {
                m_CompRefMap.erase(it);
            }
            return;
        }
    }
    assert(0);
}