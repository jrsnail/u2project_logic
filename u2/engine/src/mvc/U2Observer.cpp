//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Observer.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ObserverManager* Singleton<ObserverManager>::msSingleton = 0;
ObserverManager* ObserverManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new ObserverManager;
	}
	return msSingleton;
}
//-----------------------------------------------------------------------
ObserverManager::ObserverManager()
{
    CREATE_FACTORY(Observer);
}
//-----------------------------------------------------------------------
ObserverManager::~ObserverManager()
{
}
//-----------------------------------------------------------------------
Observer* ObserverManager::createOrReuseObserver(const String& type
    , Observer::NotifyCallbackFun callback, void* target)
{
    Observer* pObserver = reuseObject(type);
    pObserver->setCallback(callback, target);
    return pObserver;
}
//-----------------------------------------------------------------------
Observer* ObserverManager::reuseObject(const String& type, const String& name, const String& guid)
{
    return PoolingObjectManager<Observer>::reuseObject(type, name, guid);
}