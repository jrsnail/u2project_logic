//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "ApplicationMediators.h"

#include "ApplicationViewComponents.h"
#include "../cg/CgPrerequisites.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ShadeMediator::ShadeMediator(const u2::String& type, const u2::String& name)
    : CocosMediator(type, name)
{
}
//-----------------------------------------------------------------------
ShadeMediator::~ShadeMediator(void)
{

}
//-----------------------------------------------------------------------
Mediator::NotificationNames ShadeMediator::listNotificationInterests(void) const
{
    std::list<std::string> ntfs;

    return ntfs;
}
//-----------------------------------------------------------------------
void ShadeMediator::handleNotification(const Notification& notification)
{
    const u2::String& szNtfName = notification.getName();
    const void* pData = notification.getData();

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
LogoMediator::LogoMediator(const u2::String& type, const u2::String& name)
    : CocosMediator(type, name)
{
}
//-----------------------------------------------------------------------
LogoMediator::~LogoMediator(void)
{

}
//-----------------------------------------------------------------------
void LogoMediator::onRegister(void)
{
	
}
//-----------------------------------------------------------------------
void LogoMediator::onRemove(void)
{

}
//-----------------------------------------------------------------------
Mediator::NotificationNames LogoMediator::listNotificationInterests(void) const
{
	return std::list<std::string>();
}
//-----------------------------------------------------------------------
void LogoMediator::handleNotification(const Notification& notification)
{
    const u2::String& szNtfName = notification.getName();
    const void* pData = notification.getData();
}
//-----------------------------------------------------------------------
void LogoMediator::onCommonStateChanged(ViewComponent* viewComp, const u2::String& objName, const u2::String& msg)
{
    if (objName == LogoViewComponent::AN_LogoAction)
    {
        if (msg == "ActionEnd")
        {
            // trans
            getFacade(ON_Facade_Cg).sendNotification(NTF_Cg_Trans2Cg);
        }
    }
}
//-----------------------------------------------------------------------
void LogoMediator::startup(const u2::Context* context)
{
	Mediator::startup(context);

    LogoViewComponent* pLogoViewComponent = dynamic_cast<LogoViewComponent*>(m_pViewComp);
    if (pLogoViewComponent == nullptr)
    {
        return;
    }

    pLogoViewComponent->runLogoAction();
}
//-----------------------------------------------------------------------
void LogoMediator::end()
{
	Mediator::end();
}