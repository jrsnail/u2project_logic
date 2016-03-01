//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2CocosFrameListenerCollection__
#define __U2CocosFrameListenerCollection__


#include "U2Core.h"
#include "U2Mvc.h"
#include "cocos2d.h"


U2EG_NAMESPACE_BEGIN


class CocosFrameListenerCollection : public cocos2d::Ref, public FrameListenerCollection
{
public:
    CocosFrameListenerCollection();
    virtual ~CocosFrameListenerCollection(void);

public:
    virtual void registerFrameListener() override;

    virtual void unregisterFrameListener() override;

    void _onCocosUpdate(float dt);

protected:
    cocos2d::Scheduler* m_pScheduler;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2CocosFrameListenerCollection__) */
