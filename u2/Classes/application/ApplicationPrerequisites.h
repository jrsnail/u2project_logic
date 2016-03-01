//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __ApplicationPrerequisites__
#define __ApplicationPrerequisites__


#include "U2GameBase.h"


U2EG_NAMESPACE_USING


// facade
extern const Char* OT_ApplicationFacade;

// proxy

// command
extern const Char* OT_StartupCommand;
extern const Char* OT_Trans2ShadeCommand;
extern const Char* OT_Trans2LogoCommand;

// view
extern const Char* OT_ShadeMediator;
extern const Char* OT_ShadeViewComponent;
extern const Char* OT_LogoMediator;
extern const Char* OT_LogoViewComponent;

// notification
static const Char* NTF_Application_Startup = _TT("NTF_Application_Startup");
static const Char* NTF_Application_Trans2Shade = _TT("NTF_Application_Trans2Shade");
static const Char* NTF_Application_Trans2Logo = _TT("NTF_Application_Trans2Logo");

// context
extern const Char* OT_ShadeContext;
extern const Char* OT_SceneContext;
extern const Char* OT_TabContext;
extern const Char* OT_DialogContext;
extern const Char* OT_CommonContext;

// facade name
static const Char* ON_Facade_Application = _TT("ON_Facade_Application");

// context queue name
static const Char* ON_ContextQueue_Shade = _TT("ON_ContextQueue_Shade");
static const Char* ON_ContextQueue_Scene = _TT("ON_ContextQueue_Scene");
static const Char* ON_ContextQueue_Tab = _TT("ON_ContextQueue_Tab");
static const Char* ON_ContextQueue_Dialog = _TT("ON_ContextQueue_Dialog");
static const Char* ON_ContextQueue_Common = _TT("ON_ContextQueue_Common");

// context name
static const Char* ON_Context_Root = _TT("ON_Context_Root");
static const Char* ON_Context_LogoScene = _TT("ON_Context_LogoScene");

#endif /* defined(__ApplicationPrerequisites__) */
