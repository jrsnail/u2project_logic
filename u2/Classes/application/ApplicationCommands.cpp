//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "ApplicationCommands.h"

#include "ApplicationPrerequisites.h"
#include "ApplicationMediators.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
StartupCommand::StartupCommand(const u2::String& type, const u2::String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
StartupCommand::~StartupCommand()
{
}
//-----------------------------------------------------------------------
void StartupCommand::go(const Notification& notification)
{
    Notification ntf(NTF_Application_Trans2Shade);
    getFacade().broadcastNotification(ntf);

    Notification ntfLogo(NTF_Application_Trans2Logo);
    getFacade().broadcastNotification(ntfLogo);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Trans2ShadeCommand::Trans2ShadeCommand(const u2::String& type, const u2::String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
Trans2ShadeCommand::~Trans2ShadeCommand()
{
}
//-----------------------------------------------------------------------
void Trans2ShadeCommand::go(const Notification& notification)
{
    // create context tree
    u2::Context* pRoot = ContextManager::getSingleton().createObject(
        OT_Context, ON_Context_Root
        , getFacade().getName()
        , OT_ShadeMediator, "ShadeMediator"
        , OT_ShadeViewComponent, "ShadeViewComponent"
        );
    ContextProxy& contextProxy = getFacade(ON_Facade_Predefined).retrieveProxy<ContextProxy>(ON_Proxy_Context);
    contextProxy.pushBack(ON_ContextQueue_Shade, pRoot);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Trans2LogoCommand::Trans2LogoCommand(const u2::String& type, const u2::String& name)
    : TransCommand(type, name)
{
}
//-----------------------------------------------------------------------
Trans2LogoCommand::~Trans2LogoCommand()
{
}
//-----------------------------------------------------------------------
void Trans2LogoCommand::go(const Notification& notification)
{
    // create context tree
    u2::Context* pRoot = ContextManager::getSingleton().retrieveObject(ON_Context_Root);
    u2::Context* pLogo = pRoot->createChild(
        OT_Context, ON_Context_LogoScene
        , getFacade().getName()
        , OT_LogoMediator, "LogoMediator"
        , OT_LogoViewComponent, "LogoViewComponent"
        );
    ContextProxy& contextProxy = getFacade(ON_Facade_Predefined).retrieveProxy<ContextProxy>(ON_Proxy_Context);
    contextProxy.pushBack(ON_ContextQueue_Scene, pLogo, ContextQueue::eTransType::TT_Overlay);
}