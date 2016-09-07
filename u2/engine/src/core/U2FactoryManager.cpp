//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2FactoryManager.h"

#include "U2Object.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
template<> FactoryManager* Singleton<FactoryManager>::msSingleton = 0;
FactoryManager* FactoryManager::getSingletonPtr(void)
{
	return msSingleton;
}
FactoryManager& FactoryManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
FactoryManager::FactoryManager()
{
}
//-----------------------------------------------------------------------
FactoryManager::~FactoryManager()
{
	do 
	{
        ObjectFactory* pObjectFactory = nullptr;
		ObjectFactoryMap::iterator fit = mObjectFactories.begin();
		if (fit != mObjectFactories.end())
		{
            pObjectFactory = fit->second;
			mObjectFactories.erase(fit);
            if (pObjectFactory != nullptr)
            {
                U2_DELETE pObjectFactory;
            }
		}
        else
        {
            break;
        }
	} while (true);

	mObjectFactories.clear();
}
//-----------------------------------------------------------------------
void FactoryManager::addObjectFactory(ObjectFactory* factory)
{
	mObjectFactories.insert(ObjectFactoryMap::value_type(factory->getType(), factory));
}
//-----------------------------------------------------------------------
bool FactoryManager::hasObjectFactory(const String& type) const
{
	return mObjectFactories.find(type) != mObjectFactories.end();
}
//-----------------------------------------------------------------------
ObjectFactory* FactoryManager::removeObjectFactory(const String& type)
{
    ObjectFactory* pObjectFactory = nullptr;
	ObjectFactoryMap::iterator it = mObjectFactories.find(type);
	if (it != mObjectFactories.end())
	{
        pObjectFactory = it->second;
		mObjectFactories.erase(it);
	}
    return pObjectFactory;
}
//-----------------------------------------------------------------------
Object* FactoryManager::createObject(const String& type, const String& name, const String& guid)
{
	// Search factories
	ObjectFactoryMap::iterator fit = mObjectFactories.find(type);
	if (fit == mObjectFactories.end())
	{
		assert(0);
	}

	return fit->second->createInstance(name, guid);
}
//-----------------------------------------------------------------------
void FactoryManager::destroyObject(Object* obj)
{
	if (obj == nullptr)
	{
		return;
	}

	// Search factories
	ObjectFactoryMap::iterator fit = mObjectFactories.find(obj->getType());
	if (fit == mObjectFactories.end())
	{
		assert(0);
	}

	return fit->second->destroyInstance(obj);
}
