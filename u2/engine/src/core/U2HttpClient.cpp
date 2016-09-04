#include "U2HttpClient.h"

#include <curl/curl.h>
#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2FrameListenerCollection.h"
// #include "U2StreamQueue.h"
// #include "U2DataFilterStream.h"
// #include "U2StringStream.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// void HttpRequest::preRecycleByPool()
// {
// 	m_szUrl = BLANK;
// 	m_eType = HttpRequest::Type::HTTP_UNKNOWN;
// 	m_Headers.clear();
// 
// 	OutMsg::preRecycleByPool();
// }
//-----------------------------------------------------------------------
// void HttpRequest::postReuseFromPool()
// {
// 	OutMsg::postReuseFromPool();
// 
// 	m_szUrl = BLANK;
// 	m_eType = HttpRequest::Type::HTTP_UNKNOWN;
// 	m_Headers.clear();
// }
//-----------------------------------------------------------------------
bool HttpRequest::canConcate(const HttpRequest* request)
{
	do 
	{
		const HttpRequest* pOther = dynamic_cast<const HttpRequest*>(request);
		if (pOther == nullptr)
		{
			break;
		}
// 		if (this->getMsgType() != MT_PREDICTABLE && this->getMsgType() != MT_SYNCHRONOUS)
// 		{
// 			break;
// 		}
// 		if (this->getMsgType() != pOther->getMsgType())
// 		{
// 			break;
// 		}
		if (this->getUrl() != pOther->getUrl())
		{
			break;
		}
		if (this->getHttpType() != pOther->getHttpType())
		{
			break;
		}
		if (this->getHttpHeaders() != pOther->getHttpHeaders())
		{
			break;
		}
		return true;

	} while (false);

	return false;
}
//-----------------------------------------------------------------------
void HttpRequest::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpResponse::HttpResponse(const String& type, const String& name, const u2::String& guid)
    : Task(type, name, guid)
    , m_lResultCode(0L)
    , m_bSucceed(false)
{
}
//-----------------------------------------------------------------------
HttpResponse::~HttpResponse()
{
}
//-----------------------------------------------------------------------
// void HttpResponse::preRecycleByPool()
// {
//     m_Headers.clear();
//     m_lResultCode = 0L;
//     m_szErrorBuffer = BLANK;
// 
//     InMsg::preRecycleByPool();
// }
//-----------------------------------------------------------------------
// void HttpResponse::postReuseFromPool()
// {
//     InMsg::postReuseFromPool();
// 
//     m_Headers.clear();
//     m_lResultCode = 0L;
//     m_szErrorBuffer = BLANK;
// }
//-----------------------------------------------------------------------
void HttpResponse::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Configure curl's timeout property
static bool configureCURL(HttpTaskLoop* client, CURL* handle, char* errorBuffer)
{
    if (!handle) {
        return false;
    }

    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, client->getTimeoutForRead());
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, client->getTimeoutForConnect());
    if (code != CURLE_OK) {
        return false;
    }

    std::string sslCaFilename = client->getSSLVerification();
    if (sslCaFilename.empty()) {
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    else {
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(handle, CURLOPT_CAINFO, sslCaFilename.c_str());
    }

    // FIXED #3224: The subthread of CCHttpClient interrupts main thread if timeout comes.
    // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL 
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");

    return true;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
typedef size_t(*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);
//-----------------------------------------------------------------------
// Callback function used by libcurl for collect response data
static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;

    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr + sizes);

    return sizes;
}
//-----------------------------------------------------------------------
// Callback function used by libcurl for collect header data
static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;

    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr + sizes);

    return sizes;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
class CURLRaii
{
private:
    /// Instance of CURL
    CURL *m_pCurl;
    /// Keeps custom header data
    curl_slist *m_pHeaders;

public:
    CURLRaii()
        : m_pCurl(curl_easy_init())
        , m_pHeaders(nullptr)
    {
    }

    ~CURLRaii()
    {
        if (m_pCurl)
            curl_easy_cleanup(m_pCurl);
        /* free the linked list for header data */
        if (m_pHeaders)
            curl_slist_free_all(m_pHeaders);
    }

    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(m_pCurl, option, data);
    }

    /**
    * @brief Inits CURL instance for common usage
    * @param request Null not allowed
    * @param callback Response write callback
    * @param stream Response write stream
    */
    bool init(HttpTaskLoop* client, HttpRequest* request, write_callback callback, void* stream, write_callback headerCallback, void* headerStream, char* errorBuffer)
    {
        if (!m_pCurl)
            return false;
        if (!configureCURL(client, m_pCurl, errorBuffer))
            return false;

        /* get custom header data (if set) */
        const vector<String>::type& headers = request->getHttpHeaders();
        if (!headers.empty())
        {
            /* append custom headers one by one */
            for (vector<String>::const_iterator it = headers.begin(); it != headers.end(); ++it)
                m_pHeaders = curl_slist_append(m_pHeaders, it->c_str());
            /* set custom headers for curl */
            if (!setOption(CURLOPT_HTTPHEADER, m_pHeaders))
                return false;
        }
        String cookieFilename = client->getCookieFilename();
        if (!cookieFilename.empty())
        {
            if (!setOption(CURLOPT_COOKIEFILE, cookieFilename.c_str()))
            {
                return false;
            }
            if (!setOption(CURLOPT_COOKIEJAR, cookieFilename.c_str()))
            {
                return false;
            }
        }

        return setOption(CURLOPT_URL, request->getUrl().c_str())
            && setOption(CURLOPT_WRITEFUNCTION, callback)
            && setOption(CURLOPT_WRITEDATA, stream)
            && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
            && setOption(CURLOPT_HEADERDATA, headerStream);
    }

    /// @param responseCode Null not allowed
    bool perform(long *responseCode)
    {
        if (CURLE_OK != curl_easy_perform(m_pCurl))
            return false;
        // 		CURLcode code = curl_easy_getinfo(m_pCurl, CURLINFO_RESPONSE_CODE, responseCode);
        // 		if (code != CURLE_OK || !(*responseCode >= 200 && *responseCode < 300)) 
        // 		{
        // 			LogManager::getSingleton().stream(LML_NORMAL) 
        // 				<< "Curl curl_easy_getinfo failed: " 
        // 				<< curl_easy_strerror(code);
        // 			return false;
        // 		}
        // Get some more data.

        return true;
    }
};
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Process Get Request
static int processGetTask(HttpTaskLoop* client, HttpRequest* request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
    bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
        && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
        && curl.perform(responseCode);
    return ok ? 0 : 1;
}
//-----------------------------------------------------------------------
//Process POST Request
static int processPostTask(HttpTaskLoop* client, HttpRequest* request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
    bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
        && curl.setOption(CURLOPT_POST, 1)
        && curl.setOption(CURLOPT_POSTFIELDS, (request->getDataSize() == 0) ? nullptr : &(request->getData()))
        && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getDataSize())
        && curl.perform(responseCode);
    return ok ? 0 : 1;
}
//-----------------------------------------------------------------------
//Process PUT Request
static int processPutTask(HttpTaskLoop* client, HttpRequest* request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
    bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
        && curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
        && curl.setOption(CURLOPT_POSTFIELDS, (request->getDataSize() == 0) ? nullptr : &(request->getData()))
        && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getDataSize())
        && curl.perform(responseCode);
    return ok ? 0 : 1;
}
//-----------------------------------------------------------------------
//Process DELETE Request
static int processDeleteTask(HttpTaskLoop* client, HttpRequest* request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
    bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
        && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
        && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
        && curl.perform(responseCode);
    return ok ? 0 : 1;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpTaskLoop::HttpTaskLoop(const String& type, const String& name, const u2::String& guid)
    : TaskLoop(type, name, guid)
    , m_bKeepRunning(true)
    , m_uTimeoutForConnect(30)
    , m_uTimeoutForRead(60)
{
    memset(m_ResponseMessage, 0, RESPONSE_BUFFER_SIZE * sizeof(char));
}
//-----------------------------------------------------------------------
HttpTaskLoop::~HttpTaskLoop()
{
}
//-----------------------------------------------------------------------
void HttpTaskLoop::postTask(Task* task)
{
    _addToIncomingQueue(task);
}
//-----------------------------------------------------------------------
void HttpTaskLoop::postTaskAndReply(Task* task, Task* reply)
{
    assert(0);
}
//-----------------------------------------------------------------------
void HttpTaskLoop::run()
{
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    m_bKeepRunning = true;

    m_thread = std::move(std::thread(std::bind(&HttpTaskLoop::_runInternal, this)));
    //m_thread.detach();
}
//-----------------------------------------------------------------------
void HttpTaskLoop::join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}
//-----------------------------------------------------------------------
void HttpTaskLoop::quit()
{
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    m_bKeepRunning = false;
}
//-----------------------------------------------------------------------
void HttpTaskLoop::pause()
{
    quit();
    join();
}
//-----------------------------------------------------------------------
void HttpTaskLoop::resume()
{
    run();
}
//-----------------------------------------------------------------------
bool HttpTaskLoop::isRunning()
{
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    return m_bKeepRunning;
}
//-----------------------------------------------------------------------
bool HttpTaskLoop::isPausing()
{
    return !isRunning();
}
//-----------------------------------------------------------------------
String HttpTaskLoop::getThreadId()
{
    StringStream stream;
    stream << m_threadId;
    return stream.str();
}
//-----------------------------------------------------------------------
void HttpTaskLoop::processTask(HttpRequest* request, char* responseMessage)
{
    if (request == nullptr)
    {
        return;
    }
    request->serialize();

    HttpResponse* response =
        (HttpResponse*)TaskManager::getSingleton().createObject(request->getHttpResponse(), BLANK);
    if (response == nullptr)
    {
        return;
    }

    long responseCode = -1;
    int retValue = 0;

    // Process the request -> get response packet
    switch (request->getHttpType())
    {
    case HttpRequest::Type::HTTP_GET: // HTTP GET
        retValue = processGetTask(this, request,
            writeData,
            &(response->getData()),
            &responseCode,
            writeHeaderData,
            response->getHttpHeader(),
            responseMessage);
        break;

    case HttpRequest::Type::HTTP_POST: // HTTP POST
        retValue = processPostTask(this, request,
            writeData,
            &(response->getData()),
            &responseCode,
            writeHeaderData,
            response->getHttpHeader(),
            responseMessage);
        break;

    case HttpRequest::Type::HTTP_PUT:
        retValue = processPutTask(this, request,
            writeData,
            &(response->getData()),
            &responseCode,
            writeHeaderData,
            response->getHttpHeader(),
            responseMessage);
        break;

    case HttpRequest::Type::HTTP_DELETE:
        retValue = processDeleteTask(this, request,
            writeData,
            &(response->getData()),
            &responseCode,
            writeHeaderData,
            response->getHttpHeader(),
            responseMessage);
        break;

    default:
        U2Assert(true, "unknown http request type.");
        break;
    }

    // write data to HttpResponse
    response->setResultCode(responseCode);
    if (retValue != 0)
    {
        response->setSucceed(false);
        response->setErrorBuffer(responseMessage);
    }
    else
    {
        response->setSucceed(true);
    }

    _dispatchRecvTask(response);
}
//---------------------------------------------------------------------
void HttpTaskLoop::_dispatchRecvTask(Task* task)
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
//-----------------------------------------------------------------------
void HttpTaskLoop::setTimeoutForConnect(size_t value)
{
    U2_LOCK_MUTEX(m_TimeoutForConnectMutex);
    m_uTimeoutForConnect = value;
}
//-----------------------------------------------------------------------
size_t HttpTaskLoop::getTimeoutForConnect()
{
    U2_LOCK_MUTEX(m_TimeoutForConnectMutex);
    return m_uTimeoutForConnect;
}
//-----------------------------------------------------------------------
void HttpTaskLoop::setTimeoutForRead(size_t value)
{
    U2_LOCK_MUTEX(m_TimeoutForReadMutex);
    m_uTimeoutForRead = value;
}
//-----------------------------------------------------------------------
size_t HttpTaskLoop::getTimeoutForRead()
{
    U2_LOCK_MUTEX(m_TimeoutForReadMutex);
    return m_uTimeoutForRead;
}
//-----------------------------------------------------------------------
void HttpTaskLoop::setSSLVerification(const String& caFile)
{
    U2_LOCK_MUTEX(m_SslCaFilenameMutex);
    m_szSslCaFilename = caFile;
}
//-----------------------------------------------------------------------
const String& HttpTaskLoop::getSSLVerification()
{
    U2_LOCK_MUTEX(m_SslCaFilenameMutex);
    return m_szSslCaFilename;
}
//-----------------------------------------------------------------------
void HttpTaskLoop::enableCookies(const String& cookieFile)
{
    U2_LOCK_MUTEX(m_CookieFilenameMutex);
    if (cookieFile != BLANK)
    {
        m_szCookieFilename = cookieFile;
    }
    else
    {
        //m_szCookieFilename = (cocos2d::FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
    }
}
//-----------------------------------------------------------------------
const String& HttpTaskLoop::getCookieFilename()
{
    U2_LOCK_MUTEX(m_CookieFilenameMutex);
    return m_szCookieFilename;
}