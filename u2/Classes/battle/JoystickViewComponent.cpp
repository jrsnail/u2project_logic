#include "JoystickViewComponent.h"




//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
JoystickViewComponent::JoystickViewComponent(const u2::String& type, const u2::String& name)
    : CocosViewComponent(type, name)
    , m_bIsActived(false)
    , m_v2CurrentPos(cocos2d::Vec2::ZERO)
    , m_fRadius(0.0f)
    , m_bIsFollowRole(true)
    , m_pJsSpr(nullptr)
{
    initialize("Joystick.png");
}
//-----------------------------------------------------------------------
JoystickViewComponent::~JoystickViewComponent(void)
{
    deactive();
}
//-----------------------------------------------------------------------
void JoystickViewComponent::onEntered()
{
    CocosViewComponent::onEntered();
    initWithCenter(cocos2d::Vec2(300, 300), false);
}
//-----------------------------------------------------------------------
void JoystickViewComponent::onWillExit()
{
    CocosViewComponent::onWillExit();
    deactive();
}
//-----------------------------------------------------------------------
cocos2d::Vec2 JoystickViewComponent::_getJoystickCenter()
{
    const cocos2d::Size& size = ((cocos2d::Sprite*)m_pRootNode)->getContentSize();
    return cocos2d::Vec2(size.width / 2, size.height / 2);
}
//-----------------------------------------------------------------------
void JoystickViewComponent::initWithCenter(cocos2d::Vec2 centerPoint, bool isFollowRole)
{
    m_pJsSpr = cocos2d::Sprite::create("Sight.png");

    m_bIsFollowRole = isFollowRole;
    m_fRadius = ((cocos2d::Sprite*)m_pRootNode)->getContentSize().width / 2;
    if (m_bIsFollowRole)
    {
        centerPoint = cocos2d::Vec2::ZERO;
    }
    else
    {
    }
    m_v2CurrentPos = _getJoystickCenter();

    m_pJsSpr->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    m_pJsSpr->setPosition(m_v2CurrentPos);
    m_pRootNode->addChild(m_pJsSpr);
    m_pRootNode->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    m_pRootNode->setPosition(centerPoint);

    if (m_bIsFollowRole)
    {
        m_pRootNode->setVisible(false);
    }
    active();
}
//-----------------------------------------------------------------------
void JoystickViewComponent::active()
{
    if (!m_bIsActived)
    {
        m_bIsActived = true;
        u2::FrameListenerCollection::getSingleton().addFrameListener(this
            , std::bind(&JoystickViewComponent::_updatePos, this, std::placeholders::_1));
        registerEventListenerWithSceneGraphPri(cocos2d::EventListener::Type::TOUCH_ONE_BY_ONE);
    }
}
//-----------------------------------------------------------------------
void JoystickViewComponent::deactive()
{
    if (m_bIsActived)
    {
        m_bIsActived = false;
        u2::FrameListenerCollection::getSingleton().removeFrameListener(this);
        cocos2d::EventDispatcher* pDispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
        pDispatcher->removeEventListener(m_pTouchListener);
    }
}
//-----------------------------------------------------------------------
cocos2d::Vec2 JoystickViewComponent::getDirection()
{
    return cocos2d::ccpNormalize(m_v2CurrentPos - _getJoystickCenter());
}
//-----------------------------------------------------------------------
float JoystickViewComponent::getSpeedRate()
{
    return cocos2d::ccpDistance(m_v2CurrentPos, _getJoystickCenter()) / m_fRadius;
}
//-----------------------------------------------------------------------
void JoystickViewComponent::_updatePos(float dt)
{
    m_pJsSpr->setPosition(m_v2CurrentPos);
    DATAPOOL(ON_DataPool_Memory)->saveMemoryVec2Data("joystick_dir", getDirection());
    DATAPOOL(ON_DataPool_Memory)->saveMemoryFloatData("joystick_speed_rate", getSpeedRate());
}
//-----------------------------------------------------------------------
bool JoystickViewComponent::_onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev)
{
    if (!m_bIsActived)
    {
        return false;
    }

    m_pRootNode->setVisible(true);

    cocos2d::Vec2 v2Center = _getJoystickCenter();
    if (!m_bIsFollowRole)
    {
        cocos2d::Vec2 touchPoint = m_pRootNode->convertTouchToNodeSpaceAR(touch);
        if (ccpDistance(touchPoint, cocos2d::Vec2::ZERO) > m_fRadius)
        {
            return false;
        }
        else
        {
            m_v2CurrentPos = ccpAdd(v2Center, touchPoint);
        }
    }
    
    if (m_bIsFollowRole)
    {
        // bg position
        cocos2d::Vec2 touchPoint = touch->getLocationInView();
        touchPoint = cocos2d::Director::getInstance()->convertToGL(touchPoint);
        m_pRootNode->setPosition(touchPoint);

        // joystick position
        m_v2CurrentPos = v2Center;
        m_pJsSpr->setPosition(m_v2CurrentPos);
    }
    return true;
}
//-----------------------------------------------------------------------
void JoystickViewComponent::_onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev)
{
    cocos2d::Vec2 v2Center = _getJoystickCenter();
    cocos2d::Vec2 touchPoint = m_pRootNode->convertTouchToNodeSpaceAR(touch);
    if (ccpDistance(touchPoint, cocos2d::Vec2::ZERO) > m_fRadius)
    {
        m_v2CurrentPos = ccpAdd(v2Center, ccpMult(ccpNormalize(touchPoint), m_fRadius));
    }
    else
    {
        m_v2CurrentPos = ccpAdd(v2Center, touchPoint);
    }
}
//-----------------------------------------------------------------------
void JoystickViewComponent::_onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* ev)
{
    m_v2CurrentPos = _getJoystickCenter();
    if (m_bIsFollowRole)
    {
        m_pRootNode->setVisible(false);
    }
}
//-----------------------------------------------------------------------
void JoystickViewComponent::_onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* ev)
{
    m_v2CurrentPos = _getJoystickCenter();
    if (m_bIsFollowRole)
    {
        m_pRootNode->setVisible(false);
    }
}