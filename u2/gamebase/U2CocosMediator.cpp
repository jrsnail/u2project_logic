//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2CocosMediator.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CocosMediator::CocosMediator(const u2::String& type, const u2::String& name)
    : Mediator(type, name)
    , m_pScheduler(nullptr)
{
}
//-----------------------------------------------------------------------
CocosMediator::~CocosMediator(void)
{
}
//-----------------------------------------------------------------------
void CocosMediator::_registerFrameListener()
{
    if (m_pScheduler == nullptr)
    {
        m_pScheduler = cocos2d::Director::getInstance()->getScheduler();
        m_pScheduler->schedule(schedule_selector(CocosMediator::_cocosOnUpdate), this, 0.1f, CC_REPEAT_FOREVER, 0.1f, false);
    }
}
//-----------------------------------------------------------------------
void CocosMediator::_unregisterFrameListener()
{
    if (m_pScheduler != nullptr)
    {
        m_pScheduler->unschedule(schedule_selector(CocosMediator::_cocosOnUpdate), this);
        m_pScheduler = nullptr;
    }
}
//-----------------------------------------------------------------------
void CocosMediator::_cocosOnUpdate(float dt)
{
    Mediator::onUpdate(dt);
}