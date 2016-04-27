#include "U2Command.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Command::Command(const String& type, const String& name)
    : Object(type, name)
{

}
//-----------------------------------------------------------------------
Command::~Command()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> CommandManager* Singleton<CommandManager>::msSingleton = 0;
CommandManager* CommandManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new CommandManager;
	}
	return msSingleton;
}
CommandManager& CommandManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
CommandManager::CommandManager()
{
}
//-----------------------------------------------------------------------
CommandManager::~CommandManager()
{
}