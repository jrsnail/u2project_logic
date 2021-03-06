#include "U2Core.h"



U2EG_NAMESPACE_USING


void initFactroy()
{
    // data pool
    CREATE_FACTORY(DataPool);

    // task loop
    CREATE_FACTORY(ActiveHttpTaskLoop);
    CREATE_FACTORY(JsonWsTaskLoop);
    CREATE_FACTORY(LogicTaskLoop);

    // task
}