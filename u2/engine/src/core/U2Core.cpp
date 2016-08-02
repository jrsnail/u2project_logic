#include "U2Core.h"



U2EG_NAMESPACE_USING


void initFactroy()
{
    CREATE_FACTORY_WITH_TYPE(SpriteComponent, "component_sprite");
    CREATE_FACTORY_WITH_TYPE(PositionComponent, "component_position");
    CREATE_FACTORY_WITH_TYPE(VelocityComponent, "component_velocity");
    CREATE_FACTORY_WITH_TYPE(SpeedComponent, "component_speed");
    CREATE_FACTORY_WITH_TYPE(HpComponent, "component_hp");
    CREATE_FACTORY_WITH_TYPE(BaseHpComponent, "component_base_hp");
    CREATE_FACTORY_WITH_TYPE(DeltaHpComponent, "component_delta_hp");
}