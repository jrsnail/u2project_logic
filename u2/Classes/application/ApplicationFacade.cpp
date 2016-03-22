//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "ApplicationFacade.h"

#include "ApplicationCommands.h"
#include "ApplicationMediators.h"
#include "ApplicationViewComponents.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ApplicationFacade* Singleton<ApplicationFacade>::msSingleton = 0;
//-----------------------------------------------------------------------
ApplicationFacade* ApplicationFacade::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
        Facade* pFacade = FacadeManager::getSingleton().retrieveObjectByName(ON_Facade_Application);
		if (pFacade == nullptr)
		{
            pFacade = Facade::createFacade<ApplicationFacade>(ON_Facade_Application);
		}
		msSingleton = dynamic_cast<ApplicationFacade*>(pFacade);
	}
	
	return msSingleton;
}
//-----------------------------------------------------------------------
ApplicationFacade& ApplicationFacade::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ApplicationFacade::ApplicationFacade(const u2::String& type, const u2::String& name)
    : Facade(type, name)
{
    // proxy factory

    // command factory
    CREATE_FACTORY(StartupCommand);
    CREATE_FACTORY(Trans2ShadeCommand);
    CREATE_FACTORY(Trans2LogoCommand);

    // mediator factory
    CREATE_FACTORY(ShadeMediator);
    CREATE_FACTORY(LogoMediator);

    // viewcomponent factory
    CREATE_FACTORY(ShadeViewComponent);
    CREATE_FACTORY(LogoViewComponent);

}
//-----------------------------------------------------------------------
ApplicationFacade::~ApplicationFacade(void)
{
}
//-----------------------------------------------------------------------
void ApplicationFacade::initializeFacade(void)
{
    Facade::initializeFacade();

    // context
    initializeContextQueue();
}
//-----------------------------------------------------------------------
void ApplicationFacade::initializeController(void)
{
    Facade::initializeController();

	registerCommand(NTF_Application_Startup, CommandManager::getSingleton().createObject(OT_StartupCommand, BLANK));
    registerCommand(NTF_Application_Trans2Shade, CommandManager::getSingleton().createObject(OT_Trans2ShadeCommand, BLANK));
    registerCommand(NTF_Application_Trans2Logo, CommandManager::getSingleton().createObject(OT_Trans2LogoCommand, BLANK));
}
//-----------------------------------------------------------------------
void ApplicationFacade::initializeModel(void)
{
	Facade::initializeModel();
}
//-----------------------------------------------------------------------
void ApplicationFacade::initializeView(void)
{
	Facade::initializeView();
}
//-----------------------------------------------------------------------
void ApplicationFacade::initializeContextQueue(void)
{
    ContextProxy& contextProxy = getFacade(ON_Facade_Predefined).retrieveProxy<ContextProxy>(ON_Proxy_Context);
    contextProxy.createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Shade
        , ContextQueue::eTransType::TT_Overlay, ContextQueue::eBackKeyPriority::Pri_0_Shade);
    contextProxy.createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Scene
        , ContextQueue::eTransType::TT_OneByOne, ContextQueue::eBackKeyPriority::Pri_10);
    contextProxy.createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Tab
        , ContextQueue::eTransType::TT_OneByOne, ContextQueue::eBackKeyPriority::Pri_20);
    contextProxy.createContextQueue(OT_InfiniteContextQueue, ON_ContextQueue_Common
        , ContextQueue::eTransType::TT_Overlay, ContextQueue::eBackKeyPriority::Pri_50);
    contextProxy.createContextQueue(OT_InfiniteContextQueue, ON_ContextQueue_Dialog
        , ContextQueue::eTransType::TT_Cross, ContextQueue::eBackKeyPriority::Pri_100_Dialog);
}
//-----------------------------------------------------------------------
void ApplicationFacade::startup()
{
    sendNotification(NTF_Application_Startup);
    //removeCommand(Ntf_Application_Startup); //PureMVC初始化完成，注销STARUP命令
}