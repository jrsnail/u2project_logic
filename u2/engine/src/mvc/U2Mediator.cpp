//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Mediator.h"

#include "U2ViewComponent.h"
#include "U2Context.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Mediator::Mediator(const u2::String& type, const u2::String& name)
    : Object(type, name)
    , m_pViewComp(nullptr)
	, m_bCreator(false)
{
    
}
//-----------------------------------------------------------------------
Mediator::~Mediator(void)
{
}
//-----------------------------------------------------------------------
void Mediator::startup(const u2::Context* context)
{
	m_bCreator = false;
    m_pViewComp = ViewComponentManager::getSingleton().retrieveObject(context->getViewCompClass(), context->getViewCompName());
	if (m_pViewComp == nullptr)
	{
        m_pViewComp = ViewComponentManager::getSingleton().createObject(context->getViewCompClass(), context->getViewCompName());
		m_bCreator = true;
	}
    m_pViewComp->addListener(this);
}
//-----------------------------------------------------------------------
void Mediator::end()
{
    if (m_pViewComp != nullptr)
    {
        m_pViewComp->removeListener(this);

        if (m_bCreator)
        {
            ViewComponentManager::getSingleton().destoryObject(m_pViewComp);
        }
        else
        {
            m_pViewComp = nullptr;
        }
    }
}
//-----------------------------------------------------------------------
inline void Mediator::setViewComponent(ViewComponent* view_component)
{
    m_pViewComp = view_component;
}
//-----------------------------------------------------------------------
inline void const* Mediator::getViewComponent(void) const
{
    return m_pViewComp;
}
//-----------------------------------------------------------------------
inline Mediator::NotificationNames Mediator::listNotificationInterests(void) const
{
    return std::list<std::string>();
}
//-----------------------------------------------------------------------
inline void Mediator::handleNotification(const Notification& notification)
{
    (void)notification;
}
//-----------------------------------------------------------------------
inline void Mediator::onRegister(void)
{ }
//-----------------------------------------------------------------------
inline void Mediator::onRemove(void)
{ }
//-----------------------------------------------------------------------
void Mediator::onUpdate(float dt)
{ }


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> MediatorManager* Singleton<MediatorManager>::msSingleton = 0;
MediatorManager* MediatorManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new MediatorManager;
	}
    return msSingleton;
}
MediatorManager& MediatorManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
MediatorManager::MediatorManager()
{
}
//-----------------------------------------------------------------------
MediatorManager::~MediatorManager()
{
}