//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#include "TabInfoPrerequisites.h"

#include "TabInfoFacade.h"
#include "TabInfoCommands.h"
#include "TabInfoMediator.h"
#include "TabMediator.h"
#include "InfoMediator.h"
#include "TabInfoViewComponent.h"

const char* ObjectTypeTabInfoFacade = typeid(TabInfoFacade).name();
const char* ObjectTypeTabInfoCreateCommand = typeid(TabInfoCreateCommand).name();
const char* ObjectTypeTabInfoDestroyCommand = typeid(TabInfoDestroyCommand).name();
const char* ObjectTypeTabInfoMediator = typeid(TabInfoMediator).name();
const char* ObjectTypeTabMediator = typeid(TabMediator).name();
const char* ObjectTypeInfoMediator = typeid(InfoMediator).name();
const char* ObjectTypeTabInfoViewComponent = typeid(TabInfoViewComponent).name();