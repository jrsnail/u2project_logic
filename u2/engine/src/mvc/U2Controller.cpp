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


Controller::Controller(const String& type, const String& name)
    : Object(type, name)
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
        U2_LOCK_AUTO_MUTEX;
        result = m_CommandMap.find(notification.getName());
        if (result == m_CommandMap.end())
        {
            return;
        }
    } while (false);
    result->second->go(notification);
}

void Controller::registerCommand(const String& notification_name, Command* command)
{
    U2_LOCK_AUTO_MUTEX;

    if (m_CommandMap.find(notification_name) == m_CommandMap.end())
    {
        if (m_pView == nullptr)
        {
            assert(0);
            //throwException<std::runtime_error>("Cannot register command [%s]. View is null.", notification_name.c_str());
        }

        Observer *observer = ObserverManager::getSingleton().createOrReuseObserver(OT_Observer
            , std::bind(&Controller::executeCommand, this, std::placeholders::_1));
        m_pView->registerObserver(notification_name, observer);
    }
    command->initializeNotifier(m_szName);
    m_CommandMap[notification_name] = command;
}

inline bool Controller::hasCommand(const String& notification_name)
{
    U2_LOCK_AUTO_MUTEX;
    return m_CommandMap.find(notification_name) != m_CommandMap.end();
}

Command* Controller::removeCommand(const String& notification_name)
{
    CommandMap::value_type::second_type command = nullptr;

    do
    {
        U2_LOCK_AUTO_MUTEX;
        // Retrieve the named mediator
        CommandMap::iterator result = m_CommandMap.find(notification_name);

        if (result == m_CommandMap.end())
            break;

        // get mediator
        command = result->second;
        // remove the mediator from the map
        m_CommandMap.erase(result);
    } while (false);

    if (command != nullptr)
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

const Command& Controller::retrieveCommand(const String& notification_name) const
{
    return const_cast<Command const&>(static_cast<Controller const&>(*this).retrieveCommand(notification_name));
}

inline Command& Controller::retrieveCommand(const String& notification)
{
    U2_LOCK_AUTO_MUTEX;

    CommandMap::const_iterator result = m_CommandMap.find(notification);
    if (result == m_CommandMap.end())
    {
        assert(0);
        //throwException<std::runtime_error>("Cannot find any command with notification name: [%s].", notification_name.c_str());
    }

    return *result->second;
}

void Controller::removeController(const String& name)
{
    Controller* pObj = ControllerManager::getSingleton().retrieveObject(name);
    if (pObj == nullptr)
    {
        return;
    }
    ControllerManager::getSingleton().destoryObject(pObj);
}

Controller::NotificationNames Controller::listNotificationNames(void) const
{
    NotificationNames names;

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
ControllerManager& ControllerManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ControllerManager::ControllerManager()
{
}
//-----------------------------------------------------------------------
ControllerManager::~ControllerManager()
{
}