//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#include "AppPrerequisites.h"


#include "AppFacade.h"
#include "AppCommands.h"
#include "AppViewComponents.h"
#include "AppLuaTasks.h"
#include "battle/JoystickViewComponent.h"
#include "battle/BattleCommands.h"

const char* OT_AppFacade = GET_OBJECT_TYPE(AppFacade);

const char* OT_StartupCommand = GET_OBJECT_TYPE(StartupCommand);
const char* OT_Trans2ShadeCommand = GET_OBJECT_TYPE(Trans2ShadeCommand);
const char* OT_StartupLuaCommand = GET_OBJECT_TYPE(StartupLuaCommand);
const char* OT_Trans2BattleCommand = GET_OBJECT_TYPE(Trans2BattleCommand);

const char* OT_ShadeViewComponent = GET_OBJECT_TYPE(ShadeViewComponent);
const char* OT_JoystickViewComponent = GET_OBJECT_TYPE(JoystickViewComponent);

// lua to c task
const char* OT_CreateLuaScriptLuaTask = GET_OBJECT_TYPE(CreateLuaScriptLuaTask);
const char* OT_SetViewCompUiNameLuaTask = GET_OBJECT_TYPE(SetViewCompUiNameLuaTask);
const char* OT_SetViewCompEnterActinLuaTask = GET_OBJECT_TYPE(SetViewCompEnterActionLuaTask);
const char* OT_SetViewCompExitActinLuaTask = GET_OBJECT_TYPE(SetViewCompExitActionLuaTask);
const char* OT_CenterViewCompLuaTask = GET_OBJECT_TYPE(CenterViewCompLuaTask);
const char* OT_PreloadLuaTask = GET_OBJECT_TYPE(PreloadLuaTask);