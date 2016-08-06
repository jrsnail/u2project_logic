#include "U2Component.h"

#include "U2GameObject.h"
#include "U2XmlSerialize.h"
#include "U2PredefinedComponents.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Component::Component(ResourceManager* creator, const String& type, ResourceHandle handle,
    const String& group, bool isManual, ManualResourceLoader* loader)
    : Resource(creator, type, handle, group, isManual, loader)
    , Prototype(type, String("prototype_component_") + type)
    , m_pBornGameObj(nullptr)
    , m_pAttachedGameObj(nullptr)
    , m_uState(CS_None)
{
    setPrototype(this);
}
//-----------------------------------------------------------------------
Component::Component(const String& type, const String& name)
    : Resource()
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
    if (hasPrototype())
    {
        Component* pPrototype = retrievePrototype();
        Component* pComponent = ComponentManager::getSingleton()._createObject(pPrototype->getType(), name);
        pComponent->copy(*pPrototype);
        pPrototype->addInstance(pComponent);
        return pComponent;
    }
    else
    {
        assert(0);
        return nullptr;
    }
}
//-----------------------------------------------------------------------
Component* Component::cloneFromInstance(const String& name)
{
    if (hasPrototype())
    {
        Component* pComponent = ComponentManager::getSingleton()._createObject(this->getType(), name);
        pComponent->copy(*this);
        this->addInstance(pComponent);
        return pComponent;
    }
    else
    {
        assert(0);
        return nullptr;
    }
}
//-----------------------------------------------------------------------
void Component::resetFromPrototype()
{
    if (hasPrototype())
    {
        copy(*retrievePrototype());
    }
    else
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
void Component::applyToPrototype()
{
    if (hasPrototype())
    {
        retrievePrototype()->copy(*this);
    }
    else
    {
        assert(0);
    }
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
    m_uState = CS_Attaching;
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
            , isManual, loader);
    }
    else if ("component_position" == name)
    {
        return U2_NEW PositionComponent(this, name, handle, group
            , isManual, loader);
    }
    else if ("component_velocity" == name)
    {
        return U2_NEW VelocityComponent(this, name, handle, group
            , isManual, loader);
    }
    else if ("component_speed_dir" == name)
    {
        return U2_NEW SpeedDirComponent(this, name, handle, group
            , isManual, loader);
    }
    else if ("component_speed" == name)
    {
        return U2_NEW SpeedComponent(this, name, handle, group
            , isManual, loader);
    }
    else if ("component_hp" == name)
    {
        return U2_NEW HpComponent(this, name, handle, group
            , isManual, loader);
    }
    else if ("component_base_hp" == name)
    {
        return U2_NEW BaseHpComponent(this, name, handle, group
            , isManual, loader);
    }
    else if ("component_delta_hp" == name)
    {
        return U2_NEW DeltaHpComponent(this, name, handle, group
            , isManual, loader);
    }
    else if ("component_joystick" == name)
    {
        return U2_NEW JoystickComponent(this, name, handle, group
            , isManual, loader);
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
Component* ComponentManager::retrieveObjectByTN(const String& type, const String& name)
{
    return m_InstanceCollection.retrieveObjectByTN(type, name);
}
//-----------------------------------------------------------------------
TypedObjectManager<Component>::ObjectMapIterator ComponentManager::retrieveAllObjectsByType(const String& type)
{
    return m_InstanceCollection.retrieveAllObjectsByType(type);
}
//-----------------------------------------------------------------------
Component* ComponentManager::retrieveObjectByGuid(const String& guid)
{
    return m_InstanceCollection.retrieveObjectByGuid(guid);
}
//-----------------------------------------------------------------------
Component* ComponentManager::retrieveObjectByType(const String& type)
{
    return m_InstanceCollection.retrieveObjectByType(type);
}
//-----------------------------------------------------------------------
Component* ComponentManager::_createObject(const String& type, const String& name)
{
    return m_InstanceCollection.createObject(type, name);
}