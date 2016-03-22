//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "CgCommands.h"

#include "CgPrerequisites.h"
#include "CgMediators.h"
#include "../application/ApplicationPrerequisites.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Trans2CgCommand::Trans2CgCommand(const u2::String& type, const u2::String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
Trans2CgCommand::~Trans2CgCommand()
{
}
//-----------------------------------------------------------------------
void Trans2CgCommand::go(const Notification& notification)
{
    // create context tree
    u2::Context* pRoot = ContextManager::getSingleton().retrieveObjectByName(ON_Context_Root);
    u2::Context* pCg = pRoot->createChild(
        OT_Context, ON_Context_CgScene
        , getFacade().getName()
        , OT_CgMediator, "CgMediator"
        , OT_CgViewComponent, "CgViewComponent"
        );
    ContextProxy& contextProxy = getFacade(ON_Facade_Predefined).retrieveProxy<ContextProxy>(ON_Proxy_Context);
    contextProxy.pushBack(ON_ContextQueue_Scene, pCg);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Trans2StartPageCommand::Trans2StartPageCommand(const u2::String& type, const u2::String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
Trans2StartPageCommand::~Trans2StartPageCommand()
{
}
//-----------------------------------------------------------------------
void Trans2StartPageCommand::go(const Notification& notification)
{
    // create context tree
    u2::Context* pRoot = ContextManager::getSingleton().retrieveObjectByName(ON_Context_Root);
    u2::Context* pStartPage = pRoot->createChild(
        OT_Context, ON_Context_StartPageScene
        , getFacade().getName()
        , OT_StartPageMediator, "StartPageMediator"
        , OT_StartPageViewComponent, "StartPageViewComponent"
        );
    ContextProxy& contextProxy = getFacade(ON_Facade_Predefined).retrieveProxy<ContextProxy>(ON_Proxy_Context);
    contextProxy.pushBack(ON_ContextQueue_Scene, pStartPage);
}