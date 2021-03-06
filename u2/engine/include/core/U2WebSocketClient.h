﻿#ifndef __U2WebSocketClient_H__
#define __U2WebSocketClient_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "threading/U2ThreadHeaders.h"
#include "U2Singleton.h"
#include "U2Task.h"
#include "U2TaskLoop.h"
#include "U2SocketTask.h"
#include "U2HeaderPrefix.h"


struct lws_protocols;
struct lws_context;
struct lws;


U2EG_NAMESPACE_BEGIN


class Scheduler;


// test url: "ws://echo.websocket.org"
class _U2Export WsTaskLoop : public TaskLoop
{
public:
    /**
    *  State enum used to represent the Websocket state.
    */
    enum class State
    {
        CONNECTING,
        OPEN,
        CLOSING,
        CLOSED,
    };

public:
    explicit WsTaskLoop(const String& type, const String& name, const String& guid = BLANK);
    virtual ~WsTaskLoop();

    virtual void postTask(Task* task) override;

    virtual void run() override;

    virtual void quit() override;

    virtual void pause() override;

    virtual void resume() override;

    virtual String getThreadId() override;

    virtual void join() override;

    virtual bool isRunning() override;

    virtual bool isPausing() override;

    void setUrl(const String& url);
    const String& getUrl() const;

    void setHeartBeatPeriod(u2uint64 period);
    u2uint64 getHeartBeatPeriod() const;

    void addProtocol(const String& protocol);

    State getState();

    int onSocketCallback(struct lws *wsi,
        int reason, void *user, void *in, size_t len);

    void startHeartBeat();

    void stopHearBeat();

protected:
    virtual void postTaskAndReply(Task* task, Task* reply) override;

    void _runInternal();

    void _addToIncomingQueue(Task* task);

    void _connect();

    void _onRecv(struct lws *wsi,
        int reason, void *user, void *in, size_t len);

    void _onSend(struct lws *wsi,
        int reason, void *user, void *in, size_t len);

    virtual RecvSocketTask* _splitRecvTask(vector<u2char>::type& buffer, bool binary) = 0;

    virtual const String& _getWsCloseRecvTask() = 0;
    virtual const String& _getWsErrorRecvTask() = 0;
    virtual const String& _getWsOpenRecvTask() = 0;
    virtual const String& _getWsHeartBeatSendTask() = 0;

    virtual const String& _getRecvTaskLoop() = 0;

    void _dispatchRecvTask(Task* task);

protected:
    std::thread     m_thread;
    /** when detach, thread.get_id() will return back 0, 
        so we should record it here.
    */
    std::thread::id m_threadId;

    // This flag is set to false when Run should return.
    U2_MUTEX(m_KeepRunningMutex);
    bool m_bKeepRunning;

    U2_MUTEX(m_PausingMutex);
    bool m_bPausing;

    U2_MUTEX(m_DestroyingMutex);
    bool m_bDestroying;

    // Protect access to m_IncomingQueue.
    U2_MUTEX(m_mtxIncomingQueue);
    // A null terminated list which creates an incoming_queue of tasks that are
    // acquired under a mutex for processing on this instance's thread. These
    // tasks have not yet been sorted out into items for our work_queue_ vs items
    // that will be handled by the TimerManager.
    list<Task*>::type m_IncomingQueue;
    list<Task*>::type m_WorkingQueue;

    String                  m_szUrl;
    vector<String>::type    m_Protocols;
    State                   m_eState;
    vector<u2char>::type	m_RecvBuffer;

    struct ::lws_protocols* m_aWsProtocols;
    struct ::lws_context*   m_pWsContext;
    struct ::lws*           m_pWebSocket;

    u2uint64                m_ulHeartBeatPeriod;
};



#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2TypedObjectManager_H__
