//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2SimpleCommand.h"


U2EG_NAMESPACE_USING


SimpleCommand::SimpleCommand(const String& type, const String& name, const String& guid)
    : Command(type, name, guid)
{
}

SimpleCommand::~SimpleCommand()
{
}

void SimpleCommand::go(const Notification& notification)
{
    (void)notification;
}