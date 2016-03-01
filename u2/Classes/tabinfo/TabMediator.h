//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __TabMediator__
#define __TabMediator__


#include "cocos2d.h"
#include "U2Prerequisites.h"
#include "U2Mediator.h"
#include "TabInfoPrerequisites.h"


U2EG_NAMESPACE_USING


class TabMediator : public Mediator
{
public:
    TabMediator(const String& type, const String& name);

    virtual ~TabMediator(void);

    virtual void onRegister(void) override;

    virtual void onRemove(void) override;

    virtual Mediator::NotificationNames listNotificationInterests(void) const override;

    virtual void handleNotification(const Notification& notification) override;

protected:
    void _onHomeTabClicked();
    void _onMusicTabClicked();
    void _onHallTabClicked();
    void _onSettingTabClicked();
};


#endif /* defined(__TabMediator__) */
