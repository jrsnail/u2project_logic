//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __ApplicationMediators__
#define __ApplicationMediators__


#include "U2GameBase.h"


U2EG_NAMESPACE_USING


class ShadeMediator : public CocosMediator
{
public:
    ShadeMediator(const u2::String& type, const u2::String& name);

    virtual ~ShadeMediator(void);

    virtual Mediator::NotificationNames listNotificationInterests(void) const override;

    virtual void handleNotification(const Notification& notification) override;

protected:
};


class LogoMediator : public CocosMediator
{
public:
	LogoMediator(const u2::String& type, const u2::String& name);

	virtual ~LogoMediator(void);

    virtual void onRegister(void) override;

    virtual void onRemove(void) override;

    virtual Mediator::NotificationNames listNotificationInterests(void) const override;

    virtual void handleNotification(const Notification& notification) override;

    virtual void startup(const u2::Context* context) override;

	virtual void end() override;

protected:
    virtual void onCommonStateChanged(ViewComponent* viewComp, const u2::String& objName, const u2::String& msg) override;
};


#endif /* defined(__ApplicationMediators__) */
