//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "TabInfoCommands.h"

#include "U2Facade.h"
#include "TabInfoPrerequisites.h"
#include "U2ViewComponent.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TabInfoCreateCommand::TabInfoCreateCommand(const String& type, const String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
TabInfoCreateCommand::~TabInfoCreateCommand()
{
}
//-----------------------------------------------------------------------
void TabInfoCreateCommand::go(const Notification& notification)
{
    ViewComponent* pComp = ViewComponentManager::getSingleton().createObject(ObjectTypeTabInfoViewComponent, BLANK);
    Mediator* pMediator = MediatorManager::getSingleton().createObject(ObjectTypeTabInfoMediator, BLANK);
    pMediator->setViewComponent(pComp);
    getFacade().registerMediator(pMediator);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TabInfoDestroyCommand::TabInfoDestroyCommand(const String& type, const String& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
TabInfoDestroyCommand::~TabInfoDestroyCommand()
{
}
//-----------------------------------------------------------------------
void TabInfoDestroyCommand::go(const Notification& notification)
{
    
}