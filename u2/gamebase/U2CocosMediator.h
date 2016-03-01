//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2CocosMediator__
#define __U2CocosMediator__


#include "U2Core.h"
#include "U2Mvc.h"
#include "cocos2d.h"


USING_NS_CC;


class CocosMediator : public cocos2d::Ref, public u2::Mediator
{
public:
    CocosMediator(const u2::String& type, const u2::String& name);

    virtual ~CocosMediator(void);

    void _cocosOnUpdate(float dt);

protected:
    virtual void _registerFrameListener() override;

    virtual void _unregisterFrameListener() override;

protected:
    cocos2d::Scheduler* m_pScheduler;
};


#endif /* defined(__U2CocosMediator__) */
