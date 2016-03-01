//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __TabInfoCommands__
#define __TabInfoCommands__


#include "cocos2d.h"
#include "U2Prerequisites.h"
#include "U2SimpleCommand.h"


U2EG_NAMESPACE_USING


class TabInfoCreateCommand : public SimpleCommand
{
public:
    TabInfoCreateCommand(const String& type, const String& name);
    virtual ~TabInfoCreateCommand();

    virtual void go(const Notification& notification) override;
};


class TabInfoDestroyCommand : public SimpleCommand
{
public:
    TabInfoDestroyCommand(const String& type, const String& name);
    virtual ~TabInfoDestroyCommand();

    virtual void go(const Notification& notification) override;
};



#endif /* defined(__TabInfoCommands__) */
