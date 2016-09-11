#include "U2WebSocketClient.h"

#include <websocket/libwebsockets.h>
#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2Scheduler.h"


U2EG_NAMESPACE_USING



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
static void printWebSocketLog(int level, const char *line)
{
    static const char * const log_level_names[] = {
        "ERR",
        "WARN",
        "NOTICE",
        "INFO",
        "DEBUG",
        "PARSER",
        "HEADER",
        "EXTENSION",
        "CLIENT",
        "LATENCY",
    };

    char buf[30] = { 0 };
    int n;
    for (n = 0; n < LLL_COUNT; n++)
    {
        if (level != (1 << n))
            continue;
        break;
    }

    LogManager::getSingleton().stream(LML_NORMAL) << "[websocket]" << buf << line;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Wrapper for converting websocket callback from static function to member function of WebSocket class.
class WebSocketCallbackWrapper
{
public:

    static int onSocketCallback(struct lws *wsi,
    enum lws_callback_reasons reason, void *user, void *in, size_t len)
    {
        // Gets the user data from context. We know that it's a 'WebSocket' instance.
        lws_context* context = lws_get_context(wsi);
        WsTaskLoop* pWsTaskLoop = (WsTaskLoop*)lws_context_user(context);
        if (pWsTaskLoop)
        {
            return pWsTaskLoop->onSocketCallback(wsi, reason, user, in, len);
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
    , m_bDestroying(false)
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
    U2_LOCK_MUTEX_NAMED(m_DestroyingMutex, destroyingLck);
    m_bDestroying = true;

    quit();
    join();
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
    U2_LOCK_MUTEX_NAMED(m_KeepRunningMutex, runningLck);
    m_bKeepRunning = true;
    
    U2_LOCK_MUTEX_NAMED(m_PausingMutex, pausingLck);
    m_bPausing = false;

    m_thread = std::move(std::thread(std::bind(&WsTaskLoop::_runInternal, this)));
}
//-----------------------------------------------------------------------
void WsTaskLoop::quit()
{
    U2_LOCK_MUTEX_NAMED(m_KeepRunningMutex, runningLck);
    m_bKeepRunning = false;

    U2_LOCK_MUTEX_NAMED(m_PausingMutex, pausingLck);
    m_bPausing = true;
}
//-----------------------------------------------------------------------
void WsTaskLoop::join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}
//-----------------------------------------------------------------------
void WsTaskLoop::pause()
{
    quit();
    join();
}
//-----------------------------------------------------------------------
void WsTaskLoop::resume()
{
    run();
}
//-----------------------------------------------------------------------
bool WsTaskLoop::isRunning()
{
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    return m_bKeepRunning;
}
//-----------------------------------------------------------------------
bool WsTaskLoop::isPausing()
{
    U2_LOCK_MUTEX(m_PausingMutex);
    return m_bPausing;
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

        if (m_eState == State::CLOSED || m_eState == State::CLOSING)
        {
            lws_context_destroy(m_pWsContext);
            m_pWsContext = nullptr;
            m_pWebSocket = nullptr;

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
            U2_DELETE_ARRAY_T(m_aWsProtocols, lws_protocols, protocolCount + 1, MEMCATEGORY_GENERAL);
            m_aWsProtocols = nullptr;

            // exit the loop.
            break;
        }

        if (m_pWsContext && m_eState != State::CLOSED && m_eState != State::CLOSING)
        {
            lws_service(m_pWsContext, 0);
        }

        // Sleep 1 ms
        U2_THREAD_SLEEP(1);
    }

    U2_LOCK_MUTEX_NAMED(m_DestroyingMutex, destroyingLck);
    if (m_bDestroying)
    {
        return;
    }

    _postQuitCurrentTaskLoop();
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

    m_aWsProtocols = static_cast<lws_protocols*>(
        U2_NEW_ARRAY_T(lws_protocols, protocolCount + 1, MEMCATEGORY_GENERAL)
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



    static const struct lws_extension exts[] = {
        {
            "permessage-deflate",
            lws_extension_callback_pm_deflate,
        // client_no_context_takeover extension is not supported in the current version, it will cause connection fail
        // It may be a bug of lib websocket build
        //            "permessage-deflate; client_no_context_takeover; client_max_window_bits"
        "permessage-deflate; client_max_window_bits"
        },
        {
            "deflate-frame",
            lws_extension_callback_pm_deflate,
        "deflate_frame"
        },
        { nullptr, nullptr, nullptr /* terminator */ }
    };

    // create context
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);

    /*
    * create the websocket context.  This tracks open connections and
    * knows how to route any traffic and which protocol version to use,
    * and if each connection is client or server side.
    *
    * For this client-only demo, we tell it to not listen on any port.
    */

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = m_aWsProtocols;
// #ifndef LWS_NO_EXTENSIONS
//     info.extensions = lws_get_internal_extensions();
// #endif
    info.extensions = exts;
    info.gid = -1;
    info.uid = -1;
    info.options = 0;
    info.user = (void*)this;

    int log_level = LLL_ERR | LLL_WARN | LLL_NOTICE/* | LLL_INFO | LLL_DEBUG | LLL_PARSER*/ | LLL_HEADER | LLL_EXT | LLL_CLIENT | LLL_LATENCY;
    lws_set_log_level(log_level, printWebSocketLog);

    m_pWsContext = lws_create_context(&info);


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
        m_pWebSocket = lws_client_connect(m_pWsContext, szHost.c_str(), nPort, bUseSSL ? 1 : 0,
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
int WsTaskLoop::onSocketCallback(struct lws *wsi, int reason,
    void *user, void *in, size_t len)
{
    switch (reason)
    {
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        {
            m_eState = State::CLOSING;

            Task* pTask = TaskManager::getSingleton().createObject(_getWsErrorRecvTask());
            _dispatchRecvTask(pTask);
            break;
        }
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
        {
            m_eState = State::OPEN;
            Task* pTask = TaskManager::getSingleton().createObject(_getWsOpenRecvTask());
            _dispatchRecvTask(pTask);

            // start the ball rolling,
            // LWS_CALLBACK_CLIENT_WRITEABLE will come next service
            lws_callback_on_writable(wsi);
            break;
        }
        case LWS_CALLBACK_CLIENT_RECEIVE:
        {
            _onRecv(wsi, reason, user, in, len);
            break;
        }
        case LWS_CALLBACK_CLIENT_WRITEABLE:
        {
            _onSend(wsi, reason, user, in, len);
            break;
        }
        case LWS_CALLBACK_PROTOCOL_DESTROY:
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
void WsTaskLoop::_onRecv(struct lws *wsi,
    int reason, void *user, void *in, size_t len)
{
    if (in && len > 0)
    {
        m_RecvBuffer.insert(m_RecvBuffer.end(), (char*)in, (char*)in + len);
    }
    else
    {

    }

    size_t uRemainingSize = lws_remaining_packet_payload(wsi);
    int isFinalFragment = lws_is_final_fragment(wsi);

    // If no more data pending, send it to the client thread
    if (uRemainingSize == 0 && isFinalFragment)
    {
        RecvSocketTask* pTask = _splitRecvTask(m_RecvBuffer, lws_frame_is_binary(wsi));
        m_RecvBuffer.clear();

        _dispatchRecvTask(pTask);
    }
}
//---------------------------------------------------------------------
void WsTaskLoop::_onSend(struct lws *wsi,
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
            lws_callback_on_writable(wsi);
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

                int nBytesWrite = lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], n
                    , (lws_write_protocol)nWriteProtocol);

                U2_FREE(buf, MEMCATEGORY_GENERAL);

                // Buffer overrun?
                if (nBytesWrite < 0)
                {
                    LogManager::getSingleton().stream(LML_CRITICAL)
                        << "ERROR: msg("
                        << pSendTask->getName()
                        << "), lws_write return "
                        << nBytesWrite
                        << ", but it should be "
                        << n
                        << ", drop this message.\n";
                    quit();
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
    lws_callback_on_writable(wsi);
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