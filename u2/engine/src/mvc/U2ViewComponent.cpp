//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//


#include "U2ViewComponent.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
ViewComponent::ViewComponent(const String& type, const String& name, const String& guid)
    : u2::Object(type, name, guid)
    , m_ViewCompState(ViewCompState::VCS_Unloaded)
{
}
//-----------------------------------------------------------------------
ViewComponent::~ViewComponent(void)
{
    m_listeners.clear();
}
/*
//-----------------------------------------------------------------------
void ViewComponent::initializeUiName(const String& uiName)
{
    m_szUiName = uiName;
}
//-----------------------------------------------------------------------
const String& ViewComponent::getUiName() const
{
    return m_szUiName;
}
*/
//-----------------------------------------------------------------------
void ViewComponent::_changeViewCompState(ViewCompState newState)
{
    m_ViewCompState = newState;

    for (ListenerList::iterator it = m_listeners.begin();
        it != m_listeners.end(); 
        ++it)
    {
        (*it)->onViewCompStateChanged(this, newState);
    }
}
//-----------------------------------------------------------------------
void ViewComponent::addListener(Listener* listener)
{
    ListenerList::iterator it = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (it == m_listeners.end())
    {
        m_listeners.push_back(listener);
    }
}
//-----------------------------------------------------------------------
void ViewComponent::removeListener(Listener* listener)
{
    ListenerList::iterator it = std::find(m_listeners.begin(), m_listeners.end(), listener);
    if (it != m_listeners.end())
    {
        m_listeners.erase(it);
    }
}
//-----------------------------------------------------------------------
inline ViewComponent::NotificationNames ViewComponent::listNotificationInterests(void) const
{
    return std::list<String>();
}
//-----------------------------------------------------------------------
inline void ViewComponent::handleNotification(const Notification& notification)
{
    (void)notification;
}
//-----------------------------------------------------------------------
inline void ViewComponent::onRegister(void)
{ }
//-----------------------------------------------------------------------
inline void ViewComponent::onRemove(void)
{ }
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ViewComponentManager* Singleton<ViewComponentManager>::msSingleton = 0;
ViewComponentManager* ViewComponentManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new ViewComponentManager;
	}
	return msSingleton;
}
//-----------------------------------------------------------------------
ViewComponentManager::ViewComponentManager()
{
}
//-----------------------------------------------------------------------
ViewComponentManager::~ViewComponentManager()
{
}