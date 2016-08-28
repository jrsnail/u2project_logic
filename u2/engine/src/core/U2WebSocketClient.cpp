#include "U2WebSocketClient.h"

#include <websocket/libwebsockets.h>
#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2DataPool.h"
#include "U2PredefinedPrerequisites.h"
#include "U2Scheduler.h"


U2EG_NAMESPACE_USING



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Wrapper for converting websocket callback from static function to member function of WebSocket class.
class WebSocketCallbackWrapper
{
public:

    static int onSocketCallback(struct libwebsocket_context *ctx, struct libwebsocket *wsi,
    enum libwebsocket_callback_reasons reason, void *user, void *in, size_t len)
    {
        // Gets the user data from context. We know that it's a 'WebSocket' instance.
        WsTaskLoop* pWsTaskLoop = (WsTaskLoop*)libwebsocket_context_user(ctx);
        if (pWsTaskLoop)
        {
            return pWsTaskLoop->onSocketCallback(ctx, wsi, reason, user, in, len);
        }
        return 0;
    }
};

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
WsTaskLoop::WsTaskLoop(const String& type, const String& name, const String& guid)
    : TaskLoop(type, name, guid)
    , m_bKeepRunning(true)
    , m_bPausing(false)
    , m_eState(State::CONNECTING)
    , m_aWsProtocols(nullptr)
    , m_pWsContext(nullptr)
    , m_pWebSocket(nullptr)
    , m_ulHeartBeatPeriod(1000L)
{
    
}
//-----------------------------------------------------------------------
WsTaskLoop::~WsTaskLoop()
{
}
//-----------------------------------------------------------------------
void WsTaskLoop::postTask(Task* task)
{
    _addToIncomingQueue(task);
}
//-----------------------------------------------------------------------
void WsTaskLoop::postTaskAndReply(Task* task, Task* reply)
{
    assert(0);
}
//-----------------------------------------------------------------------
void WsTaskLoop::run()
{
    TaskLoop::run();
    
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    m_bKeepRunning = true;

    m_thread = std::move(std::thread(std::bind(&WsTaskLoop::_runInternal, this)));
    m_thread.detach();
}
//-----------------------------------------------------------------------
void WsTaskLoop::quit()
{
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    m_bKeepRunning = false;

    TaskLoop::quit();
}
//-----------------------------------------------------------------------
void WsTaskLoop::pause()
{
    U2_LOCK_MUTEX(m_PausingMutex);
    m_bPausing = true;
    TaskLoop::pause();
}
//-----------------------------------------------------------------------
void WsTaskLoop::resume()
{
    U2_LOCK_MUTEX(m_PausingMutex);
    m_bPausing = false;
    TaskLoop::resume();
}
//-----------------------------------------------------------------------
String WsTaskLoop::getThreadId()
{
    StringStream stream;
    stream << m_threadId;
    return stream.str();
}
//-----------------------------------------------------------------------
void WsTaskLoop::_runInternal()
{
    m_threadId = std::this_thread::get_id();
    
    _postRunCurrentTaskLoop();

    _connect();

    for (;;)
    {
        {
            U2_LOCK_MUTEX(m_KeepRunningMutex);
            if (!m_bKeepRunning)
            {
                if (m_eState != State::CLOSED && m_eState != State::CLOSING)
                {
                    m_eState = State::CLOSING;
                }
            }
        }

        /* how to ?
        {
            U2_LOCK_MUTEX(m_PausingMutex);
            if (m_bPausing)
            {
                U2_THREAD_SLEEP(1000);
                continue;
            }
        }
        */

        if (m_eState == State::CLOSED || m_eState == State::CLOSING)
        {
            libwebsocket_context_destroy(m_pWsContext);

            size_t protocolCount = 0;
            if (m_Protocols.size() > 0)
            {
                protocolCount = m_Protocols.size();
            }
            else
            {
                protocolCount = 1;
            }
            for (size_t i = 0; i < protocolCount; i++)
            {
                U2_FREE((m_aWsProtocols + i)->name, MEMCATEGORY_GENERAL);
            }
            U2_DELETE_ARRAY_T(m_aWsProtocols, libwebsocket_protocols, protocolCount + 1, MEMCATEGORY_GENERAL);
            m_aWsProtocols = nullptr;

            // exit the loop.
            break;
        }

        if (m_pWsContext && m_eState != State::CLOSED && m_eState != State::CLOSING)
        {
            libwebsocket_service(m_pWsContext, 0);
        }

        // Sleep 1 ms
        U2_THREAD_SLEEP(1);
    }
}
//---------------------------------------------------------------------
void WsTaskLoop::_addToIncomingQueue(Task* task)
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    m_IncomingQueue.push_back(task);
}
//---------------------------------------------------------------------
void WsTaskLoop::setUrl(const String& url)
{
    m_szUrl = url;
}
//---------------------------------------------------------------------
const String& WsTaskLoop::getUrl() const
{
    return m_szUrl;
}
//---------------------------------------------------------------------
void WsTaskLoop::setHeartBeatPeriod(u2uint64 period)
{
    if (m_pScheduler == nullptr)
    {
        m_ulHeartBeatPeriod = period;
    }
    else
    {
        assert(0);
    }
}
//---------------------------------------------------------------------
u2uint64 WsTaskLoop::getHeartBeatPeriod() const
{
    return m_ulHeartBeatPeriod;
}
//---------------------------------------------------------------------
void WsTaskLoop::addProtocol(const String& protocol)
{
    vector<String>::iterator it = std::find(m_Protocols.begin(), m_Protocols.end(), protocol);
    if (it != m_Protocols.end())
    {
        m_Protocols.push_back(protocol);
    }
}
//---------------------------------------------------------------------
WsTaskLoop::State WsTaskLoop::getState()
{
    return m_eState;
}
//---------------------------------------------------------------------
void WsTaskLoop::_connect()
{
    // parse url
    bool bUseSSL = false;
    String szHost = m_szUrl;
    size_t pos = 0;
    int nPort = 80;

    //ws://
    pos = szHost.find("ws://");
    if (pos == 0) szHost.erase(0, 5);

    pos = szHost.find("wss://");
    if (pos == 0)
    {
        szHost.erase(0, 6);
        bUseSSL = true;
    }

    pos = szHost.find(":");
    if (pos != String::npos) nPort = atoi(szHost.substr(pos + 1, szHost.size()).c_str());

    pos = szHost.find("/", 0);
    String path = "/";
    if (pos != String::npos) path += szHost.substr(pos + 1, szHost.size());

    pos = szHost.find(":");
    if (pos != String::npos)
    {
        szHost.erase(pos, szHost.size());
    }
    else if ((pos = szHost.find("/")) != String::npos)
    {
        szHost.erase(pos, szHost.size());
    }


    // init libwebsocket_protocols
    size_t protocolCount = 0;
    if (m_Protocols.size() > 0)
    {
        protocolCount = m_Protocols.size();
    }
    else
    {
        protocolCount = 1;
    }

    m_aWsProtocols = static_cast<libwebsocket_protocols*>(
        U2_NEW_ARRAY_T(libwebsocket_protocols, protocolCount + 1, MEMCATEGORY_GENERAL)
        );

    if (m_Protocols.size() > 0)
    {
        int i = 0;
        for (vector<String>::const_iterator it = m_Protocols.begin(); it != m_Protocols.end(); ++it, ++i)
        {
            char* name = static_cast<char*>(
                U2_MALLOC(sizeof(char) * ((*it).length() + 1), MEMCATEGORY_GENERAL)
                );
            strcpy(name, (*it).c_str());
            m_aWsProtocols[i].name = name;
            m_aWsProtocols[i].callback = WebSocketCallbackWrapper::onSocketCallback;
        }
    }
    else
    {
        char* name = static_cast<char*>(
            U2_MALLOC(sizeof(char) * 20, MEMCATEGORY_GENERAL)
            );
        strcpy(name, "default-protocol");
        m_aWsProtocols[0].name = name;
        m_aWsProtocols[0].callback = WebSocketCallbackWrapper::onSocketCallback;
    }


    // create context
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));

    /*
    * create the websocket context.  This tracks open connections and
    * knows how to route any traffic and which protocol version to use,
    * and if each connection is client or server side.
    *
    * For this client-only demo, we tell it to not listen on any port.
    */

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = m_aWsProtocols;
#ifndef LWS_NO_EXTENSIONS
    info.extensions = libwebsocket_get_internal_extensions();
#endif
    info.gid = -1;
    info.uid = -1;
    info.user = (void*)this;

    m_pWsContext = libwebsocket_create_context(&info);


    // connect
    if (nullptr != m_pWsContext)
    {
        m_eState = State::CONNECTING;
        String szName;
        for (size_t i = 0; m_aWsProtocols[i].callback != nullptr; ++i)
        {
            szName += (m_aWsProtocols[i].name);
            if (m_aWsProtocols[i + 1].callback != nullptr)
            {
                szName += ", ";
            }
        }
        m_pWebSocket = libwebsocket_client_connect(m_pWsContext, szHost.c_str(), nPort, bUseSSL ? 1 : 0,
            path.c_str(), szHost.c_str(), szHost.c_str(),
            szName.c_str(), -1);
        if (nullptr == m_pWebSocket)
        {
            m_eState = State::CLOSING;
            Task* pTask = TaskManager::getSingleton().createObject(_getWsErrorRecvTask());
            _dispatchRecvTask(pTask);
        }

    }
}
//---------------------------------------------------------------------
int WsTaskLoop::onSocketCallback(struct libwebsocket_context *ctx,
    struct libwebsocket *wsi, int reason,
    void *user, void *in, size_t len)
{
    assert(m_pWsContext == nullptr || ctx == m_pWsContext);
    assert(m_pWebSocket == nullptr || wsi == nullptr || wsi == m_pWebSocket);

    switch (reason)
    {
        case LWS_CALLBACK_DEL_POLL_FD:
        case LWS_CALLBACK_PROTOCOL_DESTROY:
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        {
            Task* pTask = nullptr;
            if (reason == LWS_CALLBACK_CLIENT_CONNECTION_ERROR
                || (reason == LWS_CALLBACK_PROTOCOL_DESTROY && m_eState == State::CONNECTING)
                || (reason == LWS_CALLBACK_DEL_POLL_FD && m_eState == State::CONNECTING)
                || (reason == LWS_CALLBACK_DEL_POLL_FD && m_eState == State::OPEN)
                )
            {
                pTask = TaskManager::getSingleton().createObject(_getWsErrorRecvTask());
                m_eState = State::CLOSING;
            }
            else if (reason == LWS_CALLBACK_PROTOCOL_DESTROY && m_eState == State::CLOSING)
            {
                pTask = TaskManager::getSingleton().createObject(_getWsCloseRecvTask());
            }

            if (pTask == nullptr)
            {
            }
            else
            {
                _dispatchRecvTask(pTask);
            }
            break;
        }
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
        {
            m_eState = State::OPEN;
            Task* pTask = TaskManager::getSingleton().createObject(_getWsOpenRecvTask());
            _dispatchRecvTask(pTask);

            // start the ball rolling,
            // LWS_CALLBACK_CLIENT_WRITEABLE will come next service
            libwebsocket_callback_on_writable(ctx, wsi);
            break;
        }
        case LWS_CALLBACK_CLIENT_RECEIVE:
        {
            _onRecv(ctx, wsi, reason, user, in, len);
            break;
        }
        case LWS_CALLBACK_CLIENT_WRITEABLE:
        {
            _onSend(ctx, wsi, reason, user, in, len);
            break;
        }
        case LWS_CALLBACK_CLOSED:
        {
            m_bKeepRunning = false;

            if (m_eState != State::CLOSED)
            {
                m_eState = State::CLOSED;
                Task* pTask = TaskManager::getSingleton().createObject(_getWsCloseRecvTask());
                _dispatchRecvTask(pTask);
            }
            break;
        }
        default:
            break;
    }

    return 0;
}
//---------------------------------------------------------------------
void WsTaskLoop::_onRecv(struct libwebsocket_context *ctx, struct libwebsocket *wsi,
    int reason, void *user, void *in, size_t len)
{
    if (in && len > 0)
    {
        m_RecvBuffer.insert(m_RecvBuffer.end(), (char*)in, (char*)in + len);
        size_t uPendingFrameDataLen = libwebsockets_remaining_packet_payload(wsi);

        if (uPendingFrameDataLen > 0)
        {
        }

        // If no more data pending, send it to the client thread
        if (uPendingFrameDataLen == 0)
        {
            RecvSocketTask* pTask = _splitRecvTask(m_RecvBuffer, lws_frame_is_binary(wsi));
            m_RecvBuffer.clear();

            _dispatchRecvTask(pTask);
        }
    }
}
//---------------------------------------------------------------------
void WsTaskLoop::_onSend(struct libwebsocket_context *ctx, struct libwebsocket *wsi,
    int reason, void *user, void *in, size_t len)
{
    if (m_WorkingQueue.empty())
    {
        bool bEmpty = true;
        {
            U2_LOCK_MUTEX(m_mtxIncomingQueue);
            bEmpty = m_IncomingQueue.empty();
            if (!bEmpty)
            {
                while (!m_IncomingQueue.empty())
                {
                    m_WorkingQueue.push_back(m_IncomingQueue.front());
                    m_IncomingQueue.pop_front();
                }
            }
        }
        if (bEmpty)
        {
            //U2_THREAD_SLEEP(1000);

            // get notified as soon as we can write again
            libwebsocket_callback_on_writable(ctx, wsi);
            return;
        }
    }

    while (!m_WorkingQueue.empty())
    {
        // here may cause a fragmentary writing task, is it ok?
        {
            U2_LOCK_MUTEX(m_KeepRunningMutex);
            if (!m_bKeepRunning)
            {
                m_eState = State::CLOSING;
                break;
            }
        }

        /* how to ?
        {
            U2_LOCK_MUTEX(m_PausingMutex);
            if (m_bPausing)
            {
                U2_THREAD_SLEEP(1000);
                continue;
            }
        }
        */

        Task* pTask = m_WorkingQueue.front();
        if (pTask == nullptr)
        {
            assert(0);
        }
        else
        {
            SendSocketTask* pSendTask = dynamic_cast<SendSocketTask*>(pTask);
            if (pSendTask == nullptr)
            {
                assert(0);
            }
            else
            {
                // serialize here
                pSendTask->serialize();

                const size_t BUFFER_SIZE = 2048;
                size_t uRemaining = pSendTask->getDataSize() - pSendTask->getIssued();
                size_t n = std::min(uRemaining, BUFFER_SIZE);
                unsigned char* buf = static_cast<unsigned char*>(
                    U2_MALLOC(sizeof(unsigned char) * (LWS_SEND_BUFFER_PRE_PADDING + n + LWS_SEND_BUFFER_POST_PADDING)
                        , MEMCATEGORY_GENERAL)
                    );
                memcpy((char*)&buf[LWS_SEND_BUFFER_PRE_PADDING], pSendTask->getData().data() + pSendTask->getIssued(), n);

                int nWriteProtocol;
                if (pSendTask->getIssued() == 0)
                {
                    nWriteProtocol = pSendTask->isBinary() ? LWS_WRITE_BINARY : LWS_WRITE_TEXT;

                    // If we have more than 1 fragment
                    if (pSendTask->getDataSize() > BUFFER_SIZE)
                    {
                        nWriteProtocol |= LWS_WRITE_NO_FIN;
                    }
                }
                else
                {
                    // we are in the middle of fragments
                    nWriteProtocol = LWS_WRITE_CONTINUATION;
                    // and if not in the last fragment
                    if (uRemaining != n)
                    {
                        nWriteProtocol |= LWS_WRITE_NO_FIN;
                    }
                }

                int nBytesWrite = libwebsocket_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], n
                    , (libwebsocket_write_protocol)nWriteProtocol);

                U2_FREE(buf, MEMCATEGORY_GENERAL);

                // Buffer overrun?
                if (nBytesWrite < 0)
                {
                    break;
                }
                // Do we have another fragments to send?
                else if (uRemaining != n)
                {
                    pSendTask->addIssued(n);
                    break;
                }
                // Safely done!
                else
                {
                    TaskManager::getSingleton().destoryObject(pSendTask);
                    m_WorkingQueue.pop_front();
                }
            }
        }
        
    }

    // get notified as soon as we can write again
    libwebsocket_callback_on_writable(ctx, wsi);
}
//---------------------------------------------------------------------
void WsTaskLoop::_dispatchRecvTask(Task* task)
{
    assert(task != nullptr);

    TaskLoop* pTaskLoop = TaskLoopManager::getSingleton().retrieveObjectByName(_getRecvTaskLoop());
    if (pTaskLoop == nullptr)
    {
        assert(0);
    }
    else
    {
        pTaskLoop->postTask(task);
    }
}
//---------------------------------------------------------------------
void WsTaskLoop::startHeartBeat()
{
    postSchedulerTask(getName() + "_scheduler"
        , _getWsHeartBeatSendTask(), BLANK
        , getHeartBeatPeriod(), true, false);
}
//---------------------------------------------------------------------
void WsTaskLoop::stopHearBeat()
{
    if (m_pScheduler != nullptr)
    {
        m_pScheduler->destoryObjectByName(getName() + "_scheduler");
    }
}