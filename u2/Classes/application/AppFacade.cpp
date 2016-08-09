#include "AppFacade.h"

#include "application/AppPrerequisites.h"
#include "application/AppCommands.h"
#include "application/AppViewComponents.h"
#include "application/AppLuaTasks.h"
#include "battle/BattleCommands.h"
#include "battle/JoystickViewComponent.h"
#include "ecs/GameComponents.h"
#include "ecs/GameSystems.h"





//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
AppFacade* AppFacade::ms_pSingleton = nullptr;
//-----------------------------------------------------------------------
AppFacade::AppFacade(const u2::String& type, const u2::String& name, const u2::String& guid)
    : Facade(type, name, guid)
{
	if (ms_pSingleton != nullptr)
	{
		assert(0);
		return;
	}

	ms_pSingleton = this;
}
//-----------------------------------------------------------------------
AppFacade::~AppFacade(void)
{
	ms_pSingleton = nullptr;
}
//-----------------------------------------------------------------------
void AppFacade::initializeFacade(void)
{
    Facade::initializeFacade();

    // context
    initializeContextQueue();
}
//-----------------------------------------------------------------------
void AppFacade::initializeController(void)
{
    Facade::initializeController();

    registerCommand(NTF_App_Startup, OT_StartupCommand);
    registerCommand(NTF_App_Trans2Shade, OT_Trans2ShadeCommand);
    registerCommand(NTF_App_StartLua, OT_StartupLuaCommand);
    registerCommand(NTF_App_Trans2Battle, OT_Trans2BattleCommand);
}
//-----------------------------------------------------------------------
void AppFacade::initializeModel(void)
{
	Facade::initializeModel();
}
//-----------------------------------------------------------------------
void AppFacade::initializeView(void)
{
	Facade::initializeView();
}
//-----------------------------------------------------------------------
void AppFacade::initializeHerald(void)
{
    Facade::initializeHerald();
}
//-----------------------------------------------------------------------
void AppFacade::initializeContextQueue(void)
{
    ContextProxy* contextProxy = getFacade(ON_Facade_Predefined).retrieveProxy<ContextProxy>(ON_Proxy_Context);
    contextProxy->createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Shade, BLANK
        , ContextQueue::eTransType::TT_Overlay, ContextQueue::eBackKeyPriority::Pri_0_Shade);
    contextProxy->createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Scene, BLANK
        , ContextQueue::eTransType::TT_OneByOne, ContextQueue::eBackKeyPriority::Pri_10);
    contextProxy->createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Tab, BLANK
        , ContextQueue::eTransType::TT_OneByOne, ContextQueue::eBackKeyPriority::Pri_20);
    contextProxy->createContextQueue(OT_InfiniteContextQueue, ON_ContextQueue_Common, BLANK
        , ContextQueue::eTransType::TT_Overlay, ContextQueue::eBackKeyPriority::Pri_50);
    contextProxy->createContextQueue(OT_InfiniteContextQueue, ON_ContextQueue_Dialog, BLANK
        , ContextQueue::eTransType::TT_Cross, ContextQueue::eBackKeyPriority::Pri_100_Dialog);
}
//-----------------------------------------------------------------------
void AppFacade::startup()
{
    sendNotification(NTF_App_Startup);

    // preload music
    //GameMusic();
}