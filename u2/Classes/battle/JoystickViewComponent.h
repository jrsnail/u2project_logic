#ifndef __JoystickViewComponent_H__
#define __JoystickViewComponent_H__


#include "U2Core.h"
#include "U2Mvc.h"
#include "cocos2d.h"
#include "U2CocosViewComponent.h"



class JoystickViewComponent : public CocosViewComponent
{
public:
    JoystickViewComponent(const u2::String& type, const u2::String& name);

    virtual ~JoystickViewComponent(void);

    virtual void onEntered() override;
    virtual void onWillExit() override;

    void initWithCenter(cocos2d::Vec2 centerPoint, bool isFollowRole);
 
    void active();
    
    void deactive();

    virtual bool _onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* ev) override;
    virtual void _onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* ev) override;
    virtual void _onTouchEnd(cocos2d::Touch* touch, cocos2d::Event* ev) override;
    virtual void _onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* ev) override;

    cocos2d::Vec2 getDirection();
    float getSpeedRate();

protected:
    void _updatePos(float dt);
    cocos2d::Vec2 _getJoystickCenter();

private:
    bool m_bIsActived;
    cocos2d::Vec2 m_v2CurrentPos;       /// current pos
    float m_fRadius;                    /// the radius of the joystick
    bool m_bIsFollowRole;               /// whether follow user click
    cocos2d::Sprite*    m_pJsSpr;
};


#endif /* defined(__JoystickViewComponent_H__) */
