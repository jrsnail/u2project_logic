//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Herald.h"

#include "U2Observer.h"
#include "U2Script.h"


U2EG_NAMESPACE_USING


Herald::Herald(const String& type, const String& name, const String& guid)
    : Object(type, name, guid)
{
    
}

inline void Herald::initializeHerald(void)
{ }

inline void Herald::registerObserver(const String& notification_name, Observer* observer)
{
    ObserverMap::value_type item(notification_name, observer);
    
    m_ObserverMap.insert(item);
}

void Herald::notifyObservers(const Notification& notification)
{
    typedef std::list<Observer*> list_observer_t;
    
    const String& name = notification.getName();
    
    list_observer_t list;
    
    do
    {
        // Find observer by name
        ObserverMap::iterator result = m_ObserverMap.find(name);
        
        // Looping and store observer
        for (; result != m_ObserverMap.end() && result->first == notification.getName(); ++result)
            list.push_back(result->second);
        
    } while (false);
    
    list_observer_t::const_iterator iter = list.begin();
    for (; iter != list.end(); ++iter)
        (*iter)->notifyObserver(notification);
}

void Herald::removeObserver(const String& notification_name, const Object* const target)
{
    typedef std::pair<ObserverMap::iterator, ObserverMap::iterator>   Pair;
    Pair pair = m_ObserverMap.equal_range(notification_name);
    for (ObserverMap::iterator it = pair.first; it != pair.second; it++)
    {
        Observer* pObserver = it->second;
        if (pObserver->compareNotifyTarget(target))
        {
            m_ObserverMap.erase(it);
            bool bExist = (ObserverManager::getSingletonPtr()->retrieveObjectByGuid(pObserver->getGuid()) != nullptr);
            if (bExist)
            {
                ObserverManager::getSingletonPtr()->recycleObject(pObserver);
            }
            else
            {
                assert(0);
            }
            return;
        }
    }
}

void Herald::registerScript(Script* script)
{
    do
    {
        // donot allow re-registration (you must to removeMediator fist)
        if (m_ScriptMap.find(script->getName()) != m_ScriptMap.end())
            return;
        
        script->initializeNotifier(m_szName);
        
        // Register the Mediator for retrieval by name
        m_ScriptMap.insert(std::make_pair(script->getName(), script));
    } while (false);
    
    Script::NotificationNames result(script->listNotificationInterests());
    for (Script::NotificationNames::iterator itr = result.begin();
         itr != result.end();
         ++itr)
    {
        // Register Mediator as Observer for its list of Notification interests
        // Create Observer referencing this mediator's handlNotification method
        Observer *observer = ObserverManager::getSingletonPtr()->createOrReuseObserver(OT_Observer
            , std::bind(&Script::handleNotification, script, std::placeholders::_1)
            , script);
        registerObserver(*itr, observer);
    }
    
    // alert the mediator that it has been registered
    script->onRegister();
}

inline Script* Herald::retrieveScript(const String& scriptName)
{
    ScriptMap::const_iterator result = m_ScriptMap.find(scriptName);
    if (result == m_ScriptMap.end())
    {
        assert(0);
        //throwException<std::runtime_error>("Cannot find any mediator with name: [%s].", mediator_name.c_str());
    }
    
    return result->second;
}

Script* Herald::removeScript(const String& scriptName)
{
    ScriptMap::value_type::second_type script = NULL;
    
    do
    {
        // Retrieve the named mediator
        ScriptMap::iterator result = m_ScriptMap.find(scriptName);
        if (result == m_ScriptMap.end())
            break;
        // Get mediator object
        script = result->second;
        // remove the mediator from the map
        m_ScriptMap.erase(result);
    } while (false);
    
    if (script != NULL)
    {
        // for every notification this mediator is interested in...
        Script::NotificationNames result(script->listNotificationInterests());
        for (Script::NotificationNames::iterator iter = result.begin();
             iter != result.end();
             ++iter)
        {
            // remove the observer linking the mediator to the notification interest
            removeObserver(*iter, script);
        }
        // Notify that it was removed
        script->onRemove();
    }
    return script;
}

inline bool Herald::hasScript(const String& scriptName)
{
    return m_ScriptMap.find(scriptName) != m_ScriptMap.end();
}

void Herald::removeHerald(const String& name)
{
    Herald* pObj = HeraldManager::getSingletonPtr()->retrieveObjectByName(name);
    if (pObj == nullptr)
    {
        return;
    }
    HeraldManager::getSingletonPtr()->destoryObject(pObj);
}

Herald::ScriptNames Herald::listScriptNames(void) const
{
    ScriptNames names;
    
    for (ScriptMap::const_iterator it = m_ScriptMap.begin();
         it != m_ScriptMap.end();
         it++)
    {
        names.push_back(it->first);
    }
    
    return names;
}

Herald::~Herald(void)
{
    removeHerald(m_szName);
    ObserverMap::const_iterator iter = m_ObserverMap.begin();
    for (; iter != m_ObserverMap.end(); ++iter)
    {
        ObserverMap::value_type::second_type observer = iter->second;
        bool bExist = (ObserverManager::getSingletonPtr()->retrieveObjectByGuid(observer->getGuid()) != nullptr);
        if (bExist)
        {
            ObserverManager::getSingletonPtr()->recycleObject(observer);
        }
        else
        {
            assert(0);
        }
    }
    m_ObserverMap.clear();
    m_ScriptMap.clear();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> HeraldManager* Singleton<HeraldManager>::msSingleton = 0;
HeraldManager* HeraldManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new HeraldManager;
	}
	return msSingleton;
}
//-----------------------------------------------------------------------
HeraldManager::HeraldManager()
{
}
//-----------------------------------------------------------------------
HeraldManager::~HeraldManager()
{
}