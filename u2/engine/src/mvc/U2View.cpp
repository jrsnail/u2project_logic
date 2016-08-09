//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2View.h"

#include "U2Observer.h"
#include "U2PredefinedPrerequisites.h"
#include "U2ViewComponent.h"


U2EG_NAMESPACE_USING


View::View(const String& type, const String& name, const String& guid)
    : Object(type, name, guid)
{
    
}

inline void View::initializeView(void)
{ }

inline void View::registerObserver(const String& notification_name, Observer* observer)
{
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

void View::registerViewComp(ViewComponent* viewComp)
{
    do
    {
        // donot allow re-registration (you must to removeMediator fist)
        if (m_ViewCompMap.find(viewComp->getName()) != m_ViewCompMap.end())
            return;

        viewComp->initializeNotifier(m_szName);

        // Register the Mediator for retrieval by name
        m_ViewCompMap.insert(std::make_pair(viewComp->getName(), viewComp));
    } while (false);

    ViewComponent::NotificationNames result(viewComp->listNotificationInterests());
    for (ViewComponent::NotificationNames::iterator itr = result.begin();
        itr != result.end();
        ++itr)
    {
        // Register Mediator as Observer for its list of Notification interests
        // Create Observer referencing this mediator's handlNotification method
        Observer *observer = ObserverManager::getSingletonPtr()->createOrReuseObserver(OT_Observer
            , std::bind(&ViewComponent::handleNotification, viewComp, std::placeholders::_1)
            , viewComp);
        registerObserver(*itr, observer);
    }
    
    // alert the mediator that it has been registered
    viewComp->onRegister();
}

inline ViewComponent* View::retrieveViewComp(const String& viewCompName)
{
    ViewCompMap::const_iterator result = m_ViewCompMap.find(viewCompName);
    if (result == m_ViewCompMap.end())
    {
        assert(0);
        //throwException<std::runtime_error>("Cannot find any mediator with name: [%s].", mediator_name.c_str());
    }

    return result->second;
}

ViewComponent* View::removeViewComp(const String& viewCompName)
{
    ViewCompMap::value_type::second_type viewComp = NULL;

    do
    {
        // Retrieve the named mediator
        ViewCompMap::iterator result = m_ViewCompMap.find(viewCompName);
        if (result == m_ViewCompMap.end())
            break;
        // Get mediator object
        viewComp = result->second;
        // remove the mediator from the map
        m_ViewCompMap.erase(result);
    } while (false);

    if (viewComp != NULL)
    {
        // for every notification this mediator is interested in...
        ViewComponent::NotificationNames result(viewComp->listNotificationInterests());
        for (ViewComponent::NotificationNames::iterator iter = result.begin();
            iter != result.end();
            ++iter)
        {
            // remove the observer linking the mediator to the notification interest
            removeObserver(*iter, (Observer*)viewComp);
        }
        // Notify that it was removed
        viewComp->onRemove();
    }
    return viewComp;
}

inline bool View::hasViewComp(const String& viewCompName)
{
    return m_ViewCompMap.find(viewCompName) != m_ViewCompMap.end();
}

void View::removeView(const String& name)
{
    View* pObj = ViewManager::getSingletonPtr()->retrieveObjectByName(name);
    if (pObj == nullptr)
    {
        return;
    }
    ViewManager::getSingletonPtr()->destoryObject(pObj);
}

View::ViewCompNames View::listViewCompNames(void) const
{
    ViewCompNames names;

    for (ViewCompMap::const_iterator it = m_ViewCompMap.begin();
        it != m_ViewCompMap.end(); 
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
    m_ViewCompMap.clear();
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
//-----------------------------------------------------------------------
ViewManager::ViewManager()
{
}
//-----------------------------------------------------------------------
ViewManager::~ViewManager()
{
}