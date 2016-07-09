//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2ObjectCollection.h"

#include "U2NameGenerator.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ObjectCollection* Singleton<ObjectCollection>::msSingleton = 0;
ObjectCollection* ObjectCollection::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new ObjectCollection;
    }
    return msSingleton;
}
ObjectCollection& ObjectCollection::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ObjectCollection::ObjectCollection()
{
}
//-----------------------------------------------------------------------
ObjectCollection::~ObjectCollection()
{
}
//-----------------------------------------------------------------------
Object* ObjectCollection::createObject(const String& type, const String& name)
{
    Object* pObj = FactoryManager::getSingleton().createObject(type, name);
    mObjects[pObj->getGuid()] = pObj;
    return pObj;
}
//-----------------------------------------------------------------------
void ObjectCollection::destoryObject(Object* obj)
{
    assert(obj != nullptr);
    ObjectMap::iterator it = mObjects.find(obj->getGuid());
    if (it != mObjects.end())
    {
        FactoryManager::getSingleton().destroyObject(obj);
        mObjects.erase(it);
    }
}
//-----------------------------------------------------------------------
Object* ObjectCollection::retrieveObjectByGuid(const String& guid)
{
    ObjectMap::iterator it = mObjects.find(guid);
    if (it != mObjects.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}