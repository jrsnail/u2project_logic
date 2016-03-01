//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2CocosFrameListenerCollection.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CocosFrameListenerCollection::CocosFrameListenerCollection()
: FrameListenerCollection()
, m_pScheduler(nullptr)
{
    registerFrameListener();
}
//-----------------------------------------------------------------------
CocosFrameListenerCollection::~CocosFrameListenerCollection(void)
{
    unregisterFrameListener();
}
//-----------------------------------------------------------------------
void CocosFrameListenerCollection::registerFrameListener()
{
    if (m_pScheduler == nullptr)
    {
        m_pScheduler = cocos2d::Director::getInstance()->getScheduler();
        m_pScheduler->schedule(schedule_selector(CocosFrameListenerCollection::_onCocosUpdate), this, 0.1f, CC_REPEAT_FOREVER, 0.1f, false);
    }
}
//-----------------------------------------------------------------------
void CocosFrameListenerCollection::unregisterFrameListener()
{
    if (m_pScheduler != nullptr)
    {
        m_pScheduler->unschedule(schedule_selector(CocosFrameListenerCollection::_onCocosUpdate), this);
        m_pScheduler = nullptr;
    }
}
//-----------------------------------------------------------------------
void CocosFrameListenerCollection::_onCocosUpdate(float dt)
{
    this->_onUpdate(dt);
}