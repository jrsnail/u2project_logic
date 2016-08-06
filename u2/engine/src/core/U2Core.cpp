#include "U2Core.h"



U2EG_NAMESPACE_USING


void initFactroy()
{
    // data pool
    CREATE_FACTORY(DataPool);

    // component
    CREATE_FACTORY_WITH_TYPE(SpriteComponent, "component_sprite");
    CREATE_FACTORY_WITH_TYPE(PositionComponent, "component_position");
    CREATE_FACTORY_WITH_TYPE(VelocityComponent, "component_velocity");
    CREATE_FACTORY_WITH_TYPE(SpeedDirComponent, "component_speed_dir");
    CREATE_FACTORY_WITH_TYPE(SpeedComponent, "component_speed");
    CREATE_FACTORY_WITH_TYPE(HpComponent, "component_hp");
    CREATE_FACTORY_WITH_TYPE(BaseHpComponent, "component_base_hp");
    CREATE_FACTORY_WITH_TYPE(DeltaHpComponent, "component_delta_hp");
    CREATE_FACTORY_WITH_TYPE(JoystickComponent, "component_joystick");

    // system
    CREATE_FACTORY_WITH_TYPE(RenderSystem, "system_render");
    CREATE_FACTORY_WITH_TYPE(InputSystem, "system_input");
    CREATE_FACTORY_WITH_TYPE(MoveSystem, "system_move");
}