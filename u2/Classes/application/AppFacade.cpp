#include "AppFacade.h"

#include "application/AppPrerequisites.h"
#include "application/AppCommands.h"
#include "application/AppViewComponents.h"
#include "application/AppLuaTasks.h"
#include "battle/BattleCommands.h"
#include "battle/JoystickViewComponent.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
AppFacade* AppFacade::ms_pSingleton = nullptr;
//-----------------------------------------------------------------------
AppFacade::AppFacade(const std::string& type, const std::string& name)
    : Facade(type, name)
{
	if (ms_pSingleton != nullptr)
	{
		assert(0);
		return;
	}

	ms_pSingleton = this;

    // proxy factory
    // command factory
    CREATE_FACTORY(StartupCommand);
    CREATE_FACTORY(Trans2ShadeCommand);
    CREATE_FACTORY(StartupLuaCommand);
    CREATE_FACTORY(Trans2BattleCommand);

    // lua script factory
    CREATE_FACTORY(CocosLuaScript);

    CREATE_FACTORY(CreateLuaScriptLuaTask);
    CREATE_FACTORY(SetViewCompUiNameLuaTask);
    CREATE_FACTORY(SetViewCompEnterActionLuaTask);
    CREATE_FACTORY(SetViewCompExitActionLuaTask);
    CREATE_FACTORY(CenterViewCompLuaTask);
    CREATE_FACTORY(PreloadLuaTask);

    CREATE_LUATASK_FACTORY(OT_C2LTask_ViewCompCreated);
    CREATE_LUATASK_FACTORY(OT_C2LTask_ButtonCliecked);
    CREATE_LUATASK_FACTORY(OT_C2LTask_TouchesBegan);
    CREATE_LUATASK_FACTORY(OT_C2LTask_TouchesMoved);
    CREATE_LUATASK_FACTORY(OT_C2LTask_TouchesEnded);
    CREATE_LUATASK_FACTORY(OT_C2LTask_TouchesCancelled);
    CREATE_LUATASK_FACTORY(OT_C2LTask_KeyPressed);
    CREATE_LUATASK_FACTORY(OT_C2LTask_KeyReleased);
    CREATE_LUATASK_FACTORY(OT_C2LTask_PreloadEnd);
    CREATE_LUATASK_FACTORY("Test_L2CType");

    // view component factory
    CREATE_FACTORY(CocosViewComponent);
    CREATE_FACTORY(ShadeViewComponent);
    CREATE_FACTORY(JoystickViewComponent);

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
    contextProxy->createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Shade
        , ContextQueue::eTransType::TT_Overlay, ContextQueue::eBackKeyPriority::Pri_0_Shade);
    contextProxy->createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Scene
        , ContextQueue::eTransType::TT_OneByOne, ContextQueue::eBackKeyPriority::Pri_10);
    contextProxy->createContextQueue(OT_SingleContextQueue, ON_ContextQueue_Tab
        , ContextQueue::eTransType::TT_OneByOne, ContextQueue::eBackKeyPriority::Pri_20);
    contextProxy->createContextQueue(OT_InfiniteContextQueue, ON_ContextQueue_Common
        , ContextQueue::eTransType::TT_Overlay, ContextQueue::eBackKeyPriority::Pri_50);
    contextProxy->createContextQueue(OT_InfiniteContextQueue, ON_ContextQueue_Dialog
        , ContextQueue::eTransType::TT_Cross, ContextQueue::eBackKeyPriority::Pri_100_Dialog);
}
//-----------------------------------------------------------------------
void AppFacade::startup()
{
    sendNotification(NTF_App_Startup);

    // preload music
    //GameMusic();
}