#include "GameComponentManager.h"

#include "GameComponents.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameComponentManager::GameComponentManager()
    : ComponentManager()
{
    ResourceGroupManager::getSingleton()._registerScriptLoader(this);
    // Register with resource group manager
    ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}
//-----------------------------------------------------------------------
GameComponentManager::~GameComponentManager()
{
    // Unregister with resource group manager
    ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
    ResourceGroupManager::getSingleton()._unregisterScriptLoader(this);
}
//-----------------------------------------------------------------------
Resource* GameComponentManager::createImpl(const u2::String& name, ResourceHandle handle,
    const u2::String& group, bool isManual, ManualResourceLoader* loader,
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