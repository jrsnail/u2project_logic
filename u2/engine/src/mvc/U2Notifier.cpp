//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Notifier.h"

#include "U2Facade.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Notifier::Notifier()
{
}
//-----------------------------------------------------------------------
Notifier::~Notifier(void)
{
}
//-----------------------------------------------------------------------
void Notifier::initializeNotifier(const String& facadeKey)
{
    m_szFacadeKey = facadeKey;
}
//-----------------------------------------------------------------------
void Notifier::sendNotification(const String& notification_name, const void* data)
{
    getFacade().sendNotification(notification_name, data);
}
//-----------------------------------------------------------------------
Facade& Notifier::getFacade(void)
{
    if (m_szFacadeKey == BLANK)
        throw std::runtime_error("Multiton key for this Notifier not yet initialized!");
	return *FacadeManager::getSingleton().retrieveObjectByName(m_szFacadeKey);
}
//-----------------------------------------------------------------------
Facade& Notifier::getFacade(const String& facadeKey)
{
    Facade* pFacade = FacadeManager::getSingleton().retrieveObjectByName(facadeKey);
    if (pFacade == nullptr)
    {
        assert(0);
    }
    return *pFacade;
}
