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
void Command::go(const Notification& notification)
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
//-----------------------------------------------------------------------
CommandManager::CommandManager()
{
}
//-----------------------------------------------------------------------
CommandManager::~CommandManager()
{
}
//-----------------------------------------------------------------------
Command* CommandManager::createObject(const String& type, const String& name)
{
    if (!u2::FactoryManager::getSingletonPtr()->hasObjectFactory(type))
    {
        u2::ObjectFactory* pObjectFactory = new u2::TemplateObjectFactory<u2::Command>(type);
        u2::FactoryManager::getSingletonPtr()->addObjectFactory(pObjectFactory);
    }
    return TypedObjectManager<Command>::createObject(type, name);
}