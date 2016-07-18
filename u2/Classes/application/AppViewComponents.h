//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __AppViewComponents__
#define __AppViewComponents__


#include "U2GameBase.h"
#include "AppPrerequisites.h"


U2EG_NAMESPACE_USING


class ShadeViewComponent : public CocosViewComponent
{
public:
    ShadeViewComponent(const std::string& type, const std::string& name);

    virtual ~ShadeViewComponent(void);

    virtual void loadUi() override;

    virtual void unloadUi() override;

    virtual ViewComponent::NotificationNames listNotificationInterests(void) const override;

    virtual void handleNotification(const Notification& notification) override;

    virtual void startup() override;

protected:
};


#endif /* defined(__TabInfoViewComponent__) */
