//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#include "ApplicationPrerequisites.h"


#include "ApplicationFacade.h"
#include "ApplicationCommands.h"
#include "ApplicationMediators.h"
#include "ApplicationViewComponents.h"
#include "GameContexts.h"

const Char* OT_ApplicationFacade = GET_OBJECT_TYPE(ApplicationFacade);

const Char* OT_StartupCommand = GET_OBJECT_TYPE(StartupCommand);
const Char* OT_Trans2ShadeCommand = GET_OBJECT_TYPE(Trans2ShadeCommand);
const Char* OT_Trans2LogoCommand = GET_OBJECT_TYPE(Trans2LogoCommand);

const Char* OT_ShadeMediator = GET_OBJECT_TYPE(ShadeMediator);
const Char* OT_LogoMediator = GET_OBJECT_TYPE(LogoMediator);

const Char* OT_ShadeViewComponent = GET_OBJECT_TYPE(ShadeViewComponent);
const Char* OT_LogoViewComponent = GET_OBJECT_TYPE(LogoViewComponent);

const Char* OT_ShadeContext = GET_OBJECT_TYPE(ShadeContext);
const Char* OT_SceneContext = GET_OBJECT_TYPE(SceneContext);
const Char* OT_TabContext = GET_OBJECT_TYPE(TabContext);
const Char* OT_DialogContext = GET_OBJECT_TYPE(DialogContext);
const Char* OT_CommonContext = GET_OBJECT_TYPE(CommonContext);
