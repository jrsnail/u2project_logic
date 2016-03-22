//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2View.h"

#include "U2Mediator.h"
#include "U2Observer.h"
#include "U2PredefinedPrerequisites.h"


U2EG_NAMESPACE_USING


View::View(const String& type, const String& name)
    : Object(type, name)
{
    
}

inline void View::initializeView(void)
{ }

inline void View::registerObserver(const String& notification_name, Observer* observer)
{
    U2_LOCK_AUTO_MUTEX;

    ObserverMap::value_type item(notification_name, observer);

    m_ObserverMap.insert(item);
}

void View::notifyObservers(Notification const& notification)
{
    typedef std::list<Observer*> list_observer_t;

    const String& name = notification.getName();

    list_observer_t list;

    do
    {
        // Scope lock for safety
        U2_LOCK_AUTO_MUTEX;

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

void View::removeObserver(const String& notification_name, const Object* const target)
{
    U2_LOCK_AUTO_MUTEX;

    ObserverMap::iterator result = m_ObserverMap.find(notification_name);

    for (; result != m_ObserverMap.end(); ++result)
    {
        if (result->first != notification_name)
            continue;

        ObserverMap::value_type::second_type observer = result->second;
        if (observer->compareNotifyTarget(target))
        {
            m_ObserverMap.erase(result);
            bool bExist = (ObserverManager::getSingleton().retrieveObjectByGuid(observer->getGuid()) != nullptr);
            if (bExist)
            {
                ObserverManager::getSingleton().recycleObject(observer);
            }
            else
            {
                assert(0);
            }
            return;
        }
    }
}

void View::registerMediator(Mediator* mediator)
{
    do
    {
        U2_LOCK_AUTO_MUTEX;

        // donot allow re-registration (you must to removeMediator fist)
        if (m_MediatorMap.find(mediator->getName()) != m_MediatorMap.end())
            return;

        mediator->initializeNotifier(m_szName);

        // Register the Mediator for retrieval by name
        m_MediatorMap.insert(std::make_pair(mediator->getName(), mediator));
    } while (false);

    Mediator::NotificationNames result(mediator->listNotificationInterests());
    for (Mediator::NotificationNames::iterator itr = result.begin();
		itr != result.end();
		++itr)
    {
        // Register Mediator as Observer for its list of Notification interests
        // Create Observer referencing this mediator's handlNotification method
        Observer *observer = ObserverManager::getSingleton().createOrReuseObserver(OT_Observer
            , std::bind(&Mediator::handleNotification, mediator, std::placeholders::_1));
		registerObserver(*itr, observer);
    }
    // alert the mediator that it has been registered
    mediator->onRegister();
}

inline Mediator const& View::retrieveMediator(const String& mediator_name) const
{
    return const_cast<Mediator const&>(static_cast<View const&>(*this).retrieveMediator(mediator_name));
}

inline Mediator& View::retrieveMediator(const String& mediator_name)
{
    U2_LOCK_AUTO_MUTEX;

    MediatorMap::const_iterator result = m_MediatorMap.find(mediator_name);
    if (result == m_MediatorMap.end())
    {
        assert(0);
        //throwException<std::runtime_error>("Cannot find any mediator with name: [%s].", mediator_name.c_str());
    }

    return *result->second;
}

Mediator* View::removeMediator(const String& mediator_name)
{
    MediatorMap::value_type::second_type mediator = NULL;

    do
    {
        U2_LOCK_AUTO_MUTEX;
        // Retrieve the named mediator
        MediatorMap::iterator result = m_MediatorMap.find(mediator_name);
        if (result == m_MediatorMap.end())
            break;
        // Get mediator object
        mediator = result->second;
        // remove the mediator from the map
        m_MediatorMap.erase(result);
    } while (false);

    if (mediator != NULL)
    {
        // for every notification this mediator is interested in...
        Mediator::NotificationNames result(mediator->listNotificationInterests());
        for (Mediator::NotificationNames::iterator iter = result.begin();
            iter != result.end();
            ++iter)
        {
            // remove the observer linking the mediator to the notification interest
            removeObserver(*iter, (Observer*)mediator);
        }
        // Notify that it was removed
        mediator->onRemove();
    }
    return mediator;
}

inline bool View::hasMediator(const String& mediator_name)
{
    U2_LOCK_AUTO_MUTEX;
    return m_MediatorMap.find(mediator_name) != m_MediatorMap.end();
}

void View::removeView(const String& name)
{
    View* pObj = ViewManager::getSingleton().retrieveObjectByName(name);
    if (pObj == nullptr)
    {
        return;
    }
    ViewManager::getSingleton().destoryObject(pObj);
}

View::MediatorNames View::listMediatorNames(void) const
{
    MediatorNames names;

    for (MediatorMap::const_iterator it = m_MediatorMap.begin();
        it != m_MediatorMap.end(); 
        it++)
    {
        names.push_back(it->first);
    }

    return names;
}

View::~View(void)
{
    removeView(m_szName);
    ObserverMap::const_iterator iter = m_ObserverMap.begin();
    for (; iter != m_ObserverMap.end(); ++iter)
    {
        ObserverMap::value_type::second_type observer = iter->second;
        bool bExist = (ObserverManager::getSingleton().retrieveObjectByGuid(observer->getGuid()) != nullptr);
        if (bExist)
        {
            ObserverManager::getSingleton().recycleObject(observer);
        }
        else
        {
            assert(0);
        }
    }
    m_ObserverMap.clear();
    m_MediatorMap.clear();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ViewManager* Singleton<ViewManager>::msSingleton = 0;
ViewManager* ViewManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new ViewManager;
	}
	return msSingleton;
}
ViewManager& ViewManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ViewManager::ViewManager()
{
}
//-----------------------------------------------------------------------
ViewManager::~ViewManager()
{
}