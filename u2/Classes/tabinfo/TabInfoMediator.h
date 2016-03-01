//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __TabInfoMediator__
#define __TabInfoMediator__


#include "cocos2d.h"
#include "U2Prerequisites.h"
#include "U2Mediator.h"
#include "TabInfoPrerequisites.h"


U2EG_NAMESPACE_USING


class TabInfoMediator : public Mediator
{
public:
    TabInfoMediator(const String& type, const String& name);

    virtual ~TabInfoMediator(void);

    virtual void onRegister(void) override;

    virtual void onRemove(void) override;

    virtual Mediator::NotificationNames listNotificationInterests(void) const override;

    virtual void handleNotification(const Notification& notification) override;

protected:
};


#endif /* defined(__TabInfoMediator__) */
