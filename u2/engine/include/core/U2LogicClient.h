#ifndef __U2LogicClient_H__
#define __U2LogicClient_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Singleton.h"
#include "U2Task.h"
#include "U2TaskLoop.h"
#include "U2HeaderPrefix.h"



U2EG_NAMESPACE_BEGIN



class _U2Export LogicTaskLoop : public TaskLoop
{
public:
    explicit LogicTaskLoop(const String& type, const String& name);
    virtual ~LogicTaskLoop();

    virtual void postTask(Task* task) override;

    virtual void run() override;

    virtual void quit() override;

    virtual void pause() override;

    virtual void resume() override;

    virtual String getThreadId() override;

protected:
    virtual void postTaskAndReply(Task* task, Task* reply) override;

    void _onUpdate(float dt);

    void _addToIncomingQueue(Task* task);

protected:
    // Protect access to m_IncomingQueue.
    U2_MUTEX(m_mtxIncomingQueue);
    // A null terminated list which creates an incoming_queue of tasks that are
    // acquired under a mutex for processing on this instance's thread. These
    // tasks have not yet been sorted out into items for our work_queue_ vs items
    // that will be handled by the TimerManager.
    list<Task*>::type m_IncomingQueue;
    list<Task*>::type m_WorkingQueue;

};


#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2LogicClient_H__
