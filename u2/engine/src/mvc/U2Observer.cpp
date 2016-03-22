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
ObserverManager& ObserverManager::getSingleton(void)
{
	return (*getSingletonPtr());
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
Observer* ObserverManager::createOrReuseObserver(const String& type, Observer::NotifyCallbackFun callback)
{
    Observer* pObserver = createOrReuseObject(type);
    pObserver->setCallback(callback);
    return pObserver;
}
//-----------------------------------------------------------------------
Observer* ObserverManager::createOrReuseObject(const String& type)
{
    return PoolingObjectManager<Observer>::reuseObjectAsName(type);
}