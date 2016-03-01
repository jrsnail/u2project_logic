//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "MacroCommand.h"


MacroCommand::MacroCommand(const String& type)
    : Command(type)
{
    initializeMacroCommand();
}

MacroCommand::~MacroCommand()
{
}

void MacroCommand::go(INotification const& notification)
{
    CommandList::iterator result = _sub_commands.begin();
    while (result != _sub_commands.end())
    {
        (*result)->initializeNotifier(_multiton_key);
        (*result)->go(notification);

        _sub_commands.erase(result);
        result = _sub_commands.begin();
    }
}

void MacroCommand::back()
{

}

bool MacroCommand::canBack()
{
    return false;
}

std::string MacroCommand::serialize()
{
    return "";
}

void MacroCommand::deserialize(const std::string& str)
{

}

void MacroCommand::initializeMacroCommand(void)
{

}

void MacroCommand::addSubCommand(Command* command)
{
    if (command == NULL)
        return;
    _sub_commands.push_back(command);
}