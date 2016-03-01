//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __InfoMediator__
#define __InfoMediator__


#include "cocos2d.h"
#include "U2Prerequisites.h"
#include "U2Mediator.h"
#include "TabInfoPrerequisites.h"


U2EG_NAMESPACE_USING


class InfoMediator : public Mediator
{
public:
    InfoMediator(const String& type, const String& name);

    virtual ~InfoMediator(void);

    virtual void onRegister(void) override;

    virtual void onRemove(void) override;

    virtual Mediator::NotificationNames listNotificationInterests(void) const override;

    virtual void handleNotification(const Notification& notification) override;

protected:
    void _onLvBtnClicked();
    void _onLifeBtnClicked();
    void _onGoldBtnClicked();
    void _onDiamondBtnClicked();
};


#endif /* defined(__InfoMediator__) */
