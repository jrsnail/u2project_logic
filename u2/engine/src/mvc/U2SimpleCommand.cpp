//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2SimpleCommand.h"


U2EG_NAMESPACE_USING


SimpleCommand::SimpleCommand(const String& type, const String& name)
    : Command(type, name)
{
}

SimpleCommand::~SimpleCommand()
{
}

void SimpleCommand::go(const Notification& notification)
{
    (void)notification;
}

void SimpleCommand::back()
{

}

bool SimpleCommand::canBack()
{
    return false;
}

String SimpleCommand::serialize()
{
    return "";
}

void SimpleCommand::deserialize(const String& str)
{

}