//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "CgMediators.h"

#include "CgViewComponents.h"
#include "../application/ApplicationPrerequisites.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CgMediator::CgMediator(const u2::String& type, const u2::String& name)
    : CocosMediator(type, name)
{
}
//-----------------------------------------------------------------------
CgMediator::~CgMediator(void)
{

}
//-----------------------------------------------------------------------
Mediator::NotificationNames CgMediator::listNotificationInterests(void) const
{
    std::list<std::string> ntfs;
    
    return ntfs;
}
//-----------------------------------------------------------------------
void CgMediator::handleNotification(const Notification& notification)
{
    const u2::String& szNtfName = notification.getName();
    const void* pData = notification.getData();

}
//-----------------------------------------------------------------------
void CgMediator::onCommonStateChanged(ViewComponent* viewComp, const u2::String& objName, const u2::String& msg)
{
    if (objName == CgViewComponent::AN_CgAction)
    {
        if (msg == "ActionEnd")
        {
            // trans
            getFacade().broadcastNotification(NTF_Cg_Trans2StartPage);
        }
    }
}
//-----------------------------------------------------------------------
void CgMediator::startup(const u2::Context* context)
{
    Mediator::startup(context);

    CgViewComponent* pCgViewComponent = dynamic_cast<CgViewComponent*>(m_pViewComp);
    if (pCgViewComponent == nullptr)
    {
        return;
    }

    pCgViewComponent->runCgAction();
}
//-----------------------------------------------------------------------
void CgMediator::end()
{
    Mediator::end();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
StartPageMediator::StartPageMediator(const u2::String& type, const u2::String& name)
    : CocosMediator(type, name)
{
}
//-----------------------------------------------------------------------
StartPageMediator::~StartPageMediator(void)
{

}
//-----------------------------------------------------------------------
Mediator::NotificationNames StartPageMediator::listNotificationInterests(void) const
{
    std::list<std::string> ntfs;

    return ntfs;
}
//-----------------------------------------------------------------------
void StartPageMediator::handleNotification(const Notification& notification)
{
    const u2::String& szNtfName = notification.getName();
    const void* pData = notification.getData();

}
//-----------------------------------------------------------------------
void StartPageMediator::startup(const u2::Context* context)
{
    Mediator::startup(context);

    StartPageViewComponent* pStartPageViewComponent = dynamic_cast<StartPageViewComponent*>(m_pViewComp);
    if (pStartPageViewComponent == nullptr)
    {
        return;
    }

//     cocos2d::ui::Button* pStartButton = dynamic_cast<cocos2d::ui::Button*>(pStartPageViewComponent->seekNodeByName(""));
//     if (pStartButton != nullptr)
//     {
//         pStartButton->addClickEventListener();
//     }
}
//-----------------------------------------------------------------------
void StartPageMediator::end()
{
    Mediator::end();
}