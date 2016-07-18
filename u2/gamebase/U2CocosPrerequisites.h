//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2CocosPrerequisites__
#define __U2CocosPrerequisites__


#include "U2Core.h"


U2EG_NAMESPACE_USING


// facade

// proxy

// command

// c to lua command
static const char* OT_C2LTask_ViewCompCreated = "OT_C2LTask_ViewCompCreated";
static const char* OT_C2LTask_ButtonCliecked = "OT_C2LTask_ButtonCliecked";
static const char* OT_C2LTask_TouchesBegan = "OT_C2LTask_TouchesBegan";
static const char* OT_C2LTask_TouchesMoved = "OT_C2LTask_TouchesMoved";
static const char* OT_C2LTask_TouchesEnded = "OT_C2LTask_TouchesEnd";
static const char* OT_C2LTask_TouchesCancelled = "OT_C2LTask_TouchesCancelled";
static const char* OT_C2LTask_KeyPressed = "OT_C2LTask_KeyPressed";
static const char* OT_C2LTask_KeyReleased = "OT_C2LTask_KeyReleased";

// lua to c command
static const char* OT_L2CTask_CreateLuaScript = "OT_L2CTask_CreateLuaScript";

// view
extern const char* OT_CocosViewComponent;

// notification
static const char* NTF_Predefined_ViewComp_Created = "NTF_Predefined_ViewComp_Created";
static const char* NTF_Predefined_Button_Clicked = "NTF_Predefined_Button_Clicked";
static const char* NTF_Predefined_Touches_Began = "NTF_Predefined_Touches_Began";
static const char* NTF_Predefined_Touches_Moved = "NTF_Predefined_Touches_Moved";
static const char* NTF_Predefined_Touches_Ended = "NTF_Predefined_Touches_Ended";
static const char* NTF_Predefined_Touches_Cancelled = "NTF_Predefined_Touches_Cancelled";
static const char* NTF_Predefined_KeyPressed = "NTF_Predefined_KeyPressed";
static const char* NTF_Predefined_KeyReleased = "NTF_Predefined_KeyReleased";

// context

// facade name

// context queue name

// context name

#endif /* defined(__U2CocosPrerequisites__) */
