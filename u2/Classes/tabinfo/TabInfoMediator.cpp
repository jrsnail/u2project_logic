//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "TabInfoMediator.h"

#include "ui/CocosGUI.h"
#include "TabInfoViewComponent.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TabInfoMediator::TabInfoMediator(const String& type, const String& name)
    : Mediator(type, name)
{
}
//-----------------------------------------------------------------------
TabInfoMediator::~TabInfoMediator(void)
{

}
//-----------------------------------------------------------------------
void TabInfoMediator::onRegister(void)
{
    const TabInfoViewComponent* pConstComp = static_cast<const TabInfoViewComponent*>(getViewComponent());
    TabInfoViewComponent* pComp = const_cast<TabInfoViewComponent*>(pConstComp);

}
//-----------------------------------------------------------------------
void TabInfoMediator::onRemove(void)
{

}
//-----------------------------------------------------------------------
Mediator::NotificationNames TabInfoMediator::listNotificationInterests(void) const
{
	return std::list<std::string>();
}
//-----------------------------------------------------------------------
void TabInfoMediator::handleNotification(const Notification& notification)
{
    const u2::String& szNtfName = notification.getName();
    const void* pData = notification.getData();
}