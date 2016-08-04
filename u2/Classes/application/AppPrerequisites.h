//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __AppPrerequisites__
#define __AppPrerequisites__


#include "U2GameBase.h"


U2EG_NAMESPACE_USING


// facade
extern const char* OT_AppFacade;

// proxy

// command
extern const char* OT_StartupCommand;
extern const char* OT_Trans2ShadeCommand;
extern const char* OT_StartupLuaCommand;
extern const char* OT_Trans2BattleCommand;

// view
extern const char* OT_ShadeViewComponent;
extern const char* OT_JoystickViewComponent;

// notification
static const char* NTF_App_Startup = "NTF_App_Startup";
static const char* NTF_App_Trans2Shade = "NTF_App_Trans2Shade";
static const char* NTF_App_StartLua = "NTF_App_StartLua";
static const char* NTF_App_Trans2Battle = "NTF_App_Trans2Battle";

// context

// facade name
static const char* ON_Facade_App = "ON_Facade_App";

// context queue name
static const char* ON_ContextQueue_Shade = "ON_ContextQueue_Shade";
static const char* ON_ContextQueue_Scene = "ON_ContextQueue_Scene";
static const char* ON_ContextQueue_Tab = "ON_ContextQueue_Tab";
static const char* ON_ContextQueue_Dialog = "ON_ContextQueue_Dialog";
static const char* ON_ContextQueue_Common = "ON_ContextQueue_Common";

// context name
static const char* ON_Context_Root = "ON_Context_Root";

// lua to c task
extern const char* OT_CreateLuaScriptLuaTask;
extern const char* OT_SetViewCompUiNameLuaTask;
extern const char* OT_SetViewCompEnterActinLuaTask;
extern const char* OT_SetViewCompExitActinLuaTask;
extern const char* OT_CenterViewCompLuaTask;
extern const char* OT_PreloadLuaTask;

// c to lua task
static const char* OT_C2LTask_PreloadEnd = "OT_C2LTask_PreloadEnd";

#endif /* defined(__ApplicationPrerequisites__) */
