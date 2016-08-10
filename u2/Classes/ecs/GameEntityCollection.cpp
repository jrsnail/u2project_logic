#include "GameEntityCollection.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> EntityCollection* Singleton<EntityCollection>::msSingleton = 0;
EntityCollection* EntityCollection::getSingletonPtr(void)
{
    return msSingleton;
}
EntityCollection& EntityCollection::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
EntityCollection::EntityCollection()
{
}
//-----------------------------------------------------------------------
EntityCollection::~EntityCollection()
{
}