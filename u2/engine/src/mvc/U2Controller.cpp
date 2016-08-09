//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Controller.h"

#include "U2Command.h"
#include "U2Observer.h"
#include "U2View.h"
#include "U2PredefinedPrerequisites.h"


U2EG_NAMESPACE_USING


Controller::Controller(const String& type, const String& name, const String& guid)
    : Object(type, name, guid)
    , m_pView(nullptr)
{
    
}

inline void Controller::initializeController(void)
{
	if (m_pView != nullptr)
	{
		return;
	}

	initializeView<View>(m_szName);
}

void Controller::executeCommand(const Notification& notification)
{
    CommandMap::iterator result = m_CommandMap.end();
    do
    {
        result = m_CommandMap.find(notification.getName());
        if (result == m_CommandMap.end())
        {
            return;
        }
    } while (false);
    Command* pCmd = CommandManager::getSingletonPtr()->createObject(result->second, BLANK);
    pCmd->initializeNotifier(m_szName);
    pCmd->go(notification);
    CommandManager::getSingletonPtr()->destoryObject(pCmd);
}

void Controller::registerCommand(const String& notification_name, const String& cmdType)
{
    if (m_CommandMap.find(notification_name) == m_CommandMap.end())
    {
        if (m_pView == nullptr)
        {
            assert(0);
            //throwException<std::runtime_error>("Cannot register command [%s]. View is null.", notification_name.c_str());
        }

        Observer *observer = ObserverManager::getSingletonPtr()->createOrReuseObserver(OT_Observer
            , std::bind(&Controller::executeCommand, this, std::placeholders::_1)
            , this);
        m_pView->registerObserver(notification_name, observer);
    }
    m_CommandMap[notification_name] = cmdType;
}

inline bool Controller::hasCommand(const String& notification_name)
{
    return m_CommandMap.find(notification_name) != m_CommandMap.end();
}

String Controller::removeCommand(const String& notification_name)
{
    CommandMap::value_type::second_type command = nullptr;

    do
    {
        // Retrieve the named mediator
        CommandMap::iterator result = m_CommandMap.find(notification_name);

        if (result == m_CommandMap.end())
            break;

        // get mediator
        command = result->second;
        // remove the mediator from the map
        m_CommandMap.erase(result);
    } while (false);

    if (command != BLANK)
    {
        if (m_pView == nullptr)
        {
            assert(0);
            //throwException<std::runtime_error>("Cannot remove command with notification name: [%s]. View is null.", notification_name.c_str());
        }
        
        // remove the observer
        m_pView->removeObserver(notification_name, this);
    }
    return command;
}

inline String Controller::retrieveCommand(const String& notification)
{
    CommandMap::const_iterator result = m_CommandMap.find(notification);
    if (result == m_CommandMap.end())
    {
        assert(0);
        //throwException<std::runtime_error>("Cannot find any command with notification name: [%s].", notification_name.c_str());
    }

    return result->second;
}

void Controller::removeController(const String& name)
{
    Controller* pObj = ControllerManager::getSingletonPtr()->retrieveObjectByName(name);
    if (pObj == nullptr)
    {
        return;
    }
    ControllerManager::getSingletonPtr()->destoryObject(pObj);
}

std::list<String> Controller::listNotificationNames(void) const
{
    std::list<String> names;
    for (CommandMap::const_iterator it = m_CommandMap.begin();
        it != m_CommandMap.end();
        it++)
    {
        names.push_back(it->first);
    }
    return names;
}

Controller::~Controller(void)
{
    //removeController(_multiton_key);
    m_CommandMap.clear();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ControllerManager* Singleton<ControllerManager>::msSingleton = 0;
ControllerManager* ControllerManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new ControllerManager;
	}
	return msSingleton;
}
//-----------------------------------------------------------------------
ControllerManager::ControllerManager()
{
}
//-----------------------------------------------------------------------
ControllerManager::~ControllerManager()
{
}
//-----------------------------------------------------------------------
Controller* ControllerManager::createObject(const String& type, const String& name, const String& guid)
{
    return SimpleObjectManager<Controller>::createObject(type, name, guid);
}
//-----------------------------------------------------------------------
Controller* ControllerManager::retrieveObjectByName(const String& name)
{
    return SimpleObjectManager<Controller>::retrieveObjectByName(name);
}