﻿//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2SimpleCommand__
#define __U2SimpleCommand__


#include "U2Core.h"
#include "U2Command.h"


U2EG_NAMESPACE_BEGIN


class SimpleCommand : public Command
{
public:
    SimpleCommand(const String& type, const String& name);
    virtual ~SimpleCommand();

    virtual void go(const Notification& notification) override;
    virtual void back() override;
    virtual bool canBack() override;
    virtual String serialize() override;
    virtual void deserialize(const String& str) override;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2SimpleCommand__) */
