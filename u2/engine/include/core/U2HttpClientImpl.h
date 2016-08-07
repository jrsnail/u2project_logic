#ifndef __U2HttpClientImpl_H__
#define __U2HttpClientImpl_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "threading/U2ThreadHeaders.h"
#include "U2HttpClient.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class _U2Export ActiveHttpTaskLoop : public HttpTaskLoop
{
public:
    ActiveHttpTaskLoop(const String& type, const String& name);
    virtual ~ActiveHttpTaskLoop();

protected:
    virtual void _runInternal() override;

    virtual void _addToIncomingQueue(Task* task) override;

    virtual const String& _getRecvTaskLoop() override;

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


#endif // __U2HttpClientImpl_H__
