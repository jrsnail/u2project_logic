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
ViewComponent::ViewComponent(const String& type, const String& name)
    : Object(type, name)
    , m_ViewCompState(ViewCompState::VCS_Unloaded)
{
}
//-----------------------------------------------------------------------
ViewComponent::~ViewComponent(void)
{
    m_listeners.clear();
}
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
void ViewComponent::_emitCommonStateChange(const String& objName, const String& msg)
{
    for (ListenerList::iterator it = m_listeners.begin();
        it != m_listeners.end();
        ++it)
    {
        (*it)->onCommonStateChanged(this, objName, msg);
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
ViewComponentManager& ViewComponentManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ViewComponentManager::ViewComponentManager()
{
}
//-----------------------------------------------------------------------
ViewComponentManager::~ViewComponentManager()
{
}