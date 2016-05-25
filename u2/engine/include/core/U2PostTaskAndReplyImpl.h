#ifndef __U2PostTaskAndReplyImpl_H__
#define __U2PostTaskAndReplyImpl_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class Task;


// Inherit from this in a class that implements PostTask appropriately
// for sending to a destination thread.
//
// Note that 'reply' will always get posted back to your current
// MessageLoop.
//
// If you're looking for a concrete implementation of
// PostTaskAndReply, you probably want base::MessageLoopProxy, or you
// may want base::WorkerPool.
class _U2Export PostTaskAndReplyImpl
{
public:
    // Implementation for MessageLoopProxy::PostTaskAndReply and
    // WorkerPool::PostTaskAndReply.
    bool PostTaskAndReply(Task* task, Task* reply);

private:
    virtual bool PostTask(Task* task) = 0;
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2PostTaskAndReplyImpl_H__
