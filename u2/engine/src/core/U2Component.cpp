#include "U2Component.h"

#include "U2GameObject.h"
#include "U2XmlSerialize.h"
#include "U2PredefinedComponents.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Component::Component(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, const String& name, bool isManual, ManualResourceLoader* loader)
    : Resource(creator, type, handle, group, isManual, loader)
    , Prototype(type, name)
    , m_pBornGameObj(nullptr)
    , m_pAttachedGameObj(nullptr)
    , m_uState(CS_None)
{
}
//-----------------------------------------------------------------------
Component::~Component()
{
    Component* pPrototype = this->retrievePrototype();
    if (pPrototype)
    {
        pPrototype->removeInstance(this);
    }
}
//-----------------------------------------------------------------------
void Component::copy(const Component& src)
{

}
//-----------------------------------------------------------------------
Component* Component::cloneFromPrototype(const String& name)
{
    Component* pPrototype = retrievePrototype();
    Component* pComponent = ComponentManager::getSingleton()._createObject(pPrototype->getType(), name);
    pComponent->copy(*pPrototype);
    pPrototype->addInstance(pComponent);
    return pComponent;
}
//-----------------------------------------------------------------------
Component* Component::cloneFromInstance(const String& name)
{
    Component* pComponent = ComponentManager::getSingleton()._createObject(this->getType(), name);
    pComponent->copy(*this);
    Component* pPrototype = retrievePrototype();
    pPrototype->addInstance(pComponent);
    return pComponent;
}
//-----------------------------------------------------------------------
void Component::resetFromPrototype()
{
    if (m_pPrototype == nullptr)
    {
        return;
    }
    copy(*m_pPrototype);
}
//-----------------------------------------------------------------------
void Component::applyToPrototype()
{
    if (m_pPrototype == nullptr)
    {
        return;
    }
    m_pPrototype->copy(*this);
}
//-----------------------------------------------------------------------
void Component::loadImpl(void)
{

}
//-----------------------------------------------------------------------
void Component::unloadImpl(void)
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
    // Loading order
    mLoadOrder = 999.0f;

    // Scripting is supported by this manager
    mScriptPatterns.push_back("*.comp");
    ResourceGroupManager::getSingleton()._registerScriptLoader(this);

    // Resource type
    mResourceType = "Component";

    // Register with resource group manager
    ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}
//-----------------------------------------------------------------------
ComponentManager::~ComponentManager()
{
    // Unregister with resource group manager
    ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}
//-----------------------------------------------------------------------
void ComponentManager::parseScript(InStreamPtr& stream, const String& groupName)
{
    XmlSerializeManager::getSingleton().load(stream, groupName);
}
//-----------------------------------------------------------------------
Resource* ComponentManager::createImpl(const String& name, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader,
    const NameValuePairList* createParams)
{
    // todo: integrate resource into object

    if ("component_sprite" == name)
    {
        return U2_NEW SpriteComponent(this, name, handle, group
            , String("prototype_component_") + name, isManual, loader);
    }
    else if ("component_position" == name)
    {
        return U2_NEW PositionComponent(this, name, handle, group
            , String("prototype_component_") + name, isManual, loader);
    }
    else if ("component_velocity" == name)
    {
        return U2_NEW VelocityComponent(this, name, handle, group
            , String("prototype_component_") + name, isManual, loader);
    }
    else if ("component_speed" == name)
    {
        return U2_NEW SpeedComponent(this, name, handle, group
            , String("prototype_component_") + name, isManual, loader);
    }
    else if ("component_hp" == name)
    {
        return U2_NEW HpComponent(this, name, handle, group
            , String("prototype_component_") + name, isManual, loader);
    }
    else if ("component_base_hp" == name)
    {
        return U2_NEW BaseHpComponent(this, name, handle, group
            , String("prototype_component_") + name, isManual, loader);
    }
    else if ("component_delta_hp" == name)
    {
        return U2_NEW DeltaHpComponent(this, name, handle, group
            , String("prototype_component_") + name, isManual, loader);
    }
    else
    {
        assert(0);
        return nullptr;
    }
}
//-----------------------------------------------------------------------
ComponentPtr ComponentManager::create(const String& name, const String& group,
    bool isManual, ManualResourceLoader* loader, const NameValuePairList* createParams)
{
    return std::dynamic_pointer_cast<Component>(
        createResource(name, group, isManual, loader, createParams));
}
//-----------------------------------------------------------------------
Component* ComponentManager::createObject(const String& type, const String& name)
{
    ResourcePtr resPtr = this->getResourceByName(type);
    if (resPtr)
    {
        Component* pPrototype = dynamic_cast<Component*>(resPtr.get());
        if (pPrototype)
        {
            Component* pObj = pPrototype->cloneFromPrototype(name);
            return pObj;
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
void ComponentManager::destoryObject(Component* obj)
{
    assert(obj);
    if (obj->isPrototype())
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
void ComponentManager::destoryObjectByName(const String& name)
{
    Component* pObj = retrieveObjectByName(name);
    destoryObject(pObj);
}
//-----------------------------------------------------------------------
Component* ComponentManager::retrieveObjectByName(const String& name)
{
    return m_InstanceCollection.retrieveObjectByName(name);
}
//-----------------------------------------------------------------------
Component* ComponentManager::retrieveObjectByGuid(const String& guid)
{
    return m_InstanceCollection.retrieveObjectByGuid(guid);
}
//-----------------------------------------------------------------------
bool ComponentManager::hasObjectByName(const String& name)
{
    return m_InstanceCollection.hasObjectByName(name);
}
//-----------------------------------------------------------------------
SimpleObjectManager<Component>::ObjectMapIterator ComponentManager::retrieveAllObjects()
{
    return m_InstanceCollection.retrieveAllObjects();
}
//-----------------------------------------------------------------------
Component* ComponentManager::_createObject(const String& type, const String& name)
{
    return m_InstanceCollection.createObject(type, name);
}