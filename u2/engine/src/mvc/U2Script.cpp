//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Script.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Script::Script(const String& type, const String& name, const String& guid)
: Object(type, name, guid)
{
}
//-----------------------------------------------------------------------
Script::~Script()
{
}
//-----------------------------------------------------------------------
inline Script::NotificationNames Script::listNotificationInterests(void)
{
    return std::list<String>();
}
//-----------------------------------------------------------------------
inline void Script::handleNotification(const Notification& notification)
{
    (void)notification;
}
//-----------------------------------------------------------------------
inline void Script::onRegister(void)
{ }
//-----------------------------------------------------------------------
inline void Script::onRemove(void)
{ }
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ScriptManager* Singleton<ScriptManager>::msSingleton = 0;
ScriptManager* ScriptManager::getSingletonPtr(void)
{
    assert(msSingleton != nullptr);
    return msSingleton;
}
//-----------------------------------------------------------------------
ScriptManager::ScriptManager()
{
}
//-----------------------------------------------------------------------
ScriptManager::~ScriptManager()
{
}
//-----------------------------------------------------------------------
Script* ScriptManager::createObject(const String& type, const String& name, const String& guid)
{
    return SimpleObjectManager<Script>::createObject(type, name, guid);
}