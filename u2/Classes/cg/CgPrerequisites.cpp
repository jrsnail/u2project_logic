//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#include "CgPrerequisites.h"


#include "CgFacade.h"
#include "CgCommands.h"
#include "CgMediators.h"
#include "CgViewComponents.h"

// facade
const char* OT_CgFacade = GET_OBJECT_TYPE(CgFacade);

// command
const char* OT_Trans2CgCommand = GET_OBJECT_TYPE(Trans2CgCommand);
const char* OT_Trans2StartPageCommand = GET_OBJECT_TYPE(Trans2StartPageCommand);

// mediator
const char* OT_CgMediator = GET_OBJECT_TYPE(CgMediator);
const char* OT_StartPageMediator = GET_OBJECT_TYPE(StartPageMediator);

// viewcomponent
const char* OT_CgViewComponent = GET_OBJECT_TYPE(CgViewComponent);
const char* OT_StartPageViewComponent = GET_OBJECT_TYPE(StartPageViewComponent);
