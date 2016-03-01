//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2PredefinedCommands__
#define __U2PredefinedCommands__


#include "U2Core.h"
#include "U2SimpleCommand.h"
#include "U2PredefinedMediators.h"


U2EG_NAMESPACE_USING


class Context;


/** Destroy Context and it's children, the Mediator and the ViewComponent
which attached to this context will be destroyed also.
@note do not call this command directly, you should control contexts with ContextProxy.
*/
class DestoryContextCommand : public SimpleCommand
{
public:
    DestoryContextCommand(const String& type, const String& name);
    virtual ~DestoryContextCommand();

    virtual void go(const Notification& notification) override;

protected:
    void _destroyContext(u2::Context* context);
};


class TransCommand : public SimpleCommand
{
public:
    TransCommand(const String& type, const String& name);
    virtual ~TransCommand();

    virtual void go(const Notification& notification) override;

protected:
    void _createMediator(const u2::Context* context, ContextQueue::eTransType type, const u2::Context* to);
};


class BackKeyCommand : public SimpleCommand
{
public:
    BackKeyCommand(const String& type, const String& name);
    virtual ~BackKeyCommand();

    virtual void go(const Notification& notification) override;

protected:
    bool _dispatchBack(u2::Context* context);
};


#endif /* defined(Trans2Commands) */
