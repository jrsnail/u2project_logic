#include "U2HttpDownloadClient3.h"

#include <curl/curl.h>
#include <regex>
#include "U2Exception.h"
#include "U2LogManager.h"
#include "cocos2d.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpDownloadRequest::HttpDownloadRequest(const String& type, const String& name)
    : Task(type, name)
    , m_nRetry(3)
    , m_nExpectedChunkCount(DefaultChunkCount)
    , m_ulTotalFileLen(0L)
    , m_bChunked(false)
{
    CREATE_FACTORY(Chunk);
}
//-----------------------------------------------------------------------
HttpDownloadRequest::~HttpDownloadRequest()
{
}
//-----------------------------------------------------------------------
void HttpDownloadRequest::run()
{

}
//-----------------------------------------------------------------------
Chunk* HttpDownloadRequest::createChunk(u2uint64 start, u2uint64 end)
{
    Chunk* pObj = dynamic_cast<Chunk*>(ObjectCollection::getSingleton().createObject(GET_OBJECT_TYPE(Chunk), BLANK));
    if (pObj == nullptr)
    {
        assert(0);
    }
    else
    {
        pObj->initialize(this, start, end);
        m_ChunkMap[pObj->getGuid()] = pObj;
    }
    return pObj;
}
//-----------------------------------------------------------------------
Chunk* HttpDownloadRequest::retrieveChunk(const String& guid)
{
    ChunkMap::iterator it = m_ChunkMap.find(guid);
    if (it == m_ChunkMap.end())
    {
        assert(0);
        return nullptr;
    }
    else
    {
        return it->second;
    }
}
//-----------------------------------------------------------------------
void HttpDownloadRequest::destroyChunk(const String& guid)
{
    ChunkMap::iterator it = m_ChunkMap.find(guid);
    if (it == m_ChunkMap.end())
    {
        assert(0);
    }
    else
    {
        ObjectCollection::getSingleton().destoryObject(it->second);
        m_ChunkMap.erase(it);
    }
}
//-----------------------------------------------------------------------
Chunk* HttpDownloadRequest::retrieveNextWaitingChunk()
{
    for (ChunkMap::iterator it = m_ChunkMap.begin(); 
        it != m_ChunkMap.end(); it++)
    {
        if (it->second->getDownloadState() == Chunk::eDownloadState::HDPS_Waiting)
        {
            return it->second;
        }
    }
    return nullptr;
}
//-----------------------------------------------------------------------
void HttpDownloadRequest::setChunked(bool chunked)
{
    m_bChunked = chunked;
}
//-----------------------------------------------------------------------
bool HttpDownloadRequest::isChunked() const
{
    return m_bChunked;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Chunk::Chunk(const String& type, const String& name)
    : Object(type, name)
    , m_ulStart(0L)
    , m_ulEnd(0L)
    , m_pRequest(nullptr)
    , m_eDownloadState(HDPS_Unkown)
{

}
//-----------------------------------------------------------------------
Chunk::~Chunk()
{

}
//-----------------------------------------------------------------------
void Chunk::initialize(HttpDownloadRequest* request, u2uint64 start, u2uint64 end)
{
    m_pRequest = request;
    m_ulStart = start;
    m_ulEnd = end;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
static size_t receiveHeaderFunction(char *buffer, size_t size, size_t nmemb, void *stream)
{
    u2::String* szReceiveHeader = reinterpret_cast<u2::String*>(stream);
    if (szReceiveHeader && buffer)
    {
        szReceiveHeader->append(reinterpret_cast<const char*>(buffer), size * nmemb);
    }

    return nmemb * size;
}
//-----------------------------------------------------------------------
static size_t receiveBodyFunction(char *ptr, size_t size, size_t nmemb, void *stream)
{
    u2::String* szReceiveContent = reinterpret_cast<u2::String*>(stream);
    if (szReceiveContent && ptr)
    {
        szReceiveContent->append(reinterpret_cast<const char*>(ptr), size * nmemb);
    }

    return nmemb * size;
}
//-----------------------------------------------------------------------
static size_t downloadCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    HttpDownloadRequest::HttpDownloadResponse::Chunk* chunk = reinterpret_cast<HttpDownloadRequest::HttpDownloadResponse::Chunk*>(userdata);
    HttpDownloadRequest::HttpDownloadResponse& response = chunk->request->response();

    if (response.isCanceled())
    {
        return 0;
    }

    U2_LOCK_MUTEX(response.m_DownloadMutex);

    size_t written = 0;
    size_t real_size = size * nmemb;
    if (chunk->end > 0)
    {
        if (chunk->start <= chunk->end)
        {
            if (chunk->start + real_size > chunk->end)
            {
                real_size = chunk->end - chunk->start + 1;
            }
        }
    }

    int seek_error = fseek(response.fp, chunk->start, SEEK_SET);
    if (seek_error != 0)
    {
        perror("fseek");
    }
    else
    {
        written = fwrite(ptr, 1, real_size, response.fp);
    }
    response.appendDownloadLength(written);
    chunk->start += written;

    return written;
}
//-----------------------------------------------------------------------
static int progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    HttpDownloader::DownloadHelper::ThreadChunk* thread_chunk = reinterpret_cast<HttpDownloader::DownloadHelper::ThreadChunk*>(clientp);

    DoHttpLock http_lock(thread_chunk->_download->m_httplock);

    double total_size = thread_chunk->_download->m_total_size;
    double downloaded_size = thread_chunk->_download->m_downloaded_size;
    void* userdata = thread_chunk->_download->m_userdata;
    int callback_result = thread_chunk->_download->m_download_callback(total_size, downloaded_size, userdata);

    return callback_result;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpDownloadTaskLoop::HttpDownloadTaskLoop(const String& type, const String& name)
    : TaskLoop(type, name)
    , m_bKeepRunning(true)
    , m_uTimeoutForConnect(30)
    , m_uTimeoutForRead(60)
    , m_uTotalThreadCount(1)
    , m_uMaxThreadCountPerRequest(1)
{
}
//-----------------------------------------------------------------------
HttpDownloadTaskLoop::~HttpDownloadTaskLoop()
{
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::postTask(Task* task)
{
    _addToIncomingQueue(task);
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::postTaskAndReply(Task* task, Task* reply)
{
    assert(0);
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::run()
{
    for (size_t i = 0; i < m_uTotalThreadCount; i++)
    {
        std::thread t = std::move(std::thread(std::bind(&HttpDownloadTaskLoop::_runInternal, this)));
        m_ThreadMap[t.get_id()] = std::move(t);
        m_ThreadMap[t.get_id()].detach();
    }
    
    TaskLoop::run();
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::quit()
{
    m_bKeepRunning = false;

    TaskLoop::quit();
}
//-----------------------------------------------------------------------
String HttpDownloadTaskLoop::getThreadId()
{
    StringStream stream;
    stream << m_thread.get_id();
    return stream.str();
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::setTimeoutForConnect(size_t value)
{
    U2_LOCK_MUTEX(m_TimeoutForConnectMutex);
    m_uTimeoutForConnect = value;
}
//-----------------------------------------------------------------------
size_t HttpDownloadTaskLoop::getTimeoutForConnect()
{
    U2_LOCK_MUTEX(m_TimeoutForConnectMutex);
    return m_uTimeoutForConnect;
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::setTimeoutForRead(size_t value)
{
    U2_LOCK_MUTEX(m_TimeoutForReadMutex);
    m_uTimeoutForRead = value;
}
//-----------------------------------------------------------------------
size_t HttpDownloadTaskLoop::getTimeoutForRead()
{
    U2_LOCK_MUTEX(m_TimeoutForReadMutex);
    return m_uTimeoutForRead;
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::setSSLVerification(const String& caFile)
{
    U2_LOCK_MUTEX(m_SslCaFilenameMutex);
    m_szSslCaFilename = caFile;
}
//-----------------------------------------------------------------------
const String& HttpDownloadTaskLoop::getSSLVerification()
{
    U2_LOCK_MUTEX(m_SslCaFilenameMutex);
    return m_szSslCaFilename;
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::enableCookies(const String& cookieFile)
{
    U2_LOCK_MUTEX(m_CookieFilenameMutex);
    if (cookieFile != BLANK)
    {
        m_szCookieFilename = cookieFile;
    }
    else
    {
        m_szCookieFilename = (cocos2d::FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
    }
}
//-----------------------------------------------------------------------
const String& HttpDownloadTaskLoop::getCookieFilename()
{
    U2_LOCK_MUTEX(m_CookieFilenameMutex);
    return m_szCookieFilename;
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::setTotalThreadCount(size_t count)
{
    U2_LOCK_MUTEX(m_TotalThreadCountMutex);
    m_uTotalThreadCount = count;
}
//-----------------------------------------------------------------------
size_t HttpDownloadTaskLoop::getTotalThreadCount()
{
    U2_LOCK_MUTEX(m_TotalThreadCountMutex);
    return m_uTotalThreadCount;
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::setMaxThreadCountPerRequest(size_t count)
{
    U2_LOCK_MUTEX(m_MaxThreadCountPerRequestMutex);
    m_uMaxThreadCountPerRequest = count;
}
//-----------------------------------------------------------------------
size_t HttpDownloadTaskLoop::getMaxThreadCountPerRequest()
{
    U2_LOCK_MUTEX(m_MaxThreadCountPerRequestMutex);
    return m_uMaxThreadCountPerRequest;
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::_addToIncomingQueue(Task* task)
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    m_IncomingQueue.push_back(task);
    m_ChunksSync.notify_all();
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::_runInternal()
{
    m_bKeepRunning = true;

    for (;;)
    {
        U2_LOCK_MUTEX(m_ChunksMutex);
        Chunk* pChunk = _getNextWaitingChunk();
        if (pChunk == nullptr)
        {
            U2_THREAD_WAIT(m_ChunksSync, lck);
            continue;
        }
        else
        {
            
        }

        if (!m_bKeepRunning)
            break;
    }
}
//-----------------------------------------------------------------------
Chunk* HttpDownloadTaskLoop::_getNextWaitingChunk()
{
    HttpDownloadRequest* request = nullptr;

    // find first waiting chunk/request
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    for (list<Task*>::iterator it = m_IncomingQueue.begin(); it != m_IncomingQueue.end(); it++)
    {
        HttpDownloadRequest* req = dynamic_cast<HttpDownloadRequest*>(*it);
        if (req != nullptr)
        {
            if (req->isChunked())
            {
                Chunk* pChunk = req->retrieveNextWaitingChunk();
                if (pChunk == nullptr)
                {
                    continue;
                }
                else
                {
                    return pChunk;
                }
            }
            else
            {
                request = req;
                break;
            }
        }
        else
        {
            assert(0);
        }
    }

    if (request != nullptr)
    {
        // get file total length
        String szRespHeader;
        u2uint64 ulTotalLen = _getFileLength(request, szRespHeader);

        // if match "Content-Range: bytes 2-1449/26620", means server support multi thread download
        std::regex pattern("CONTENT-RANGE\\s*:\\s*\\w+\\s*(\\d+)-(\\d*)/(\\d+)", std::regex::icase);
        bool bSupport = std::regex_search(szRespHeader, pattern);
        size_t uCaledCount = bSupport ? _splitDownloadChunks(ulTotalLen) : 1;
        size_t uExpectedCount = request->getExpectedChunkCount();
        size_t uActualChunkCount = uCaledCount > uExpectedCount ? uExpectedCount : uCaledCount;
        if (uActualChunkCount > 1)
        {
            for (size_t i = 0; i < uActualChunkCount; i++)
            {
                u2uint64 gap = static_cast<long>(ulTotalLen) / uActualChunkCount;

                if (i < uActualChunkCount - 1)
                {
                    request->createChunk(i * gap, (i + 1)* gap - 1);
                }
                else
                {
                    request->createChunk(i * gap, -1);
                }
            }
        }
        else
        {
            request->createChunk(0, 0);
        }
        request->setChunked(true);

        
        return request->retrieveNextWaitingChunk();
    }
    
    return nullptr;
}
//-----------------------------------------------------------------------
u2uint64 HttpDownloadTaskLoop::_getFileLength(HttpDownloadRequest* request, String& header)
{
    return 0L;
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::processTask(HttpDownloadRequest* request, char* responseMessage)
{
    HttpDownloadRequest::HttpDownloadResponse& response = request->response();

    double dTotalSize = _getDownloadFileSize(request);
    if (dTotalSize < 0.0)
    {
        assert(0);
        return;
    }

    std::string out_file_name = m_file_path;
    std::string src_file_name = out_file_name;
    out_file_name += ".dl";

    FILE *fp = nullptr;
#ifdef _WIN32
    DeleteFileA(out_file_name.c_str());
    fopen_s(&fp, out_file_name.c_str(), "wb");
#else
    unlink(out_file_name.c_str());
    fp = fopen(out_file_name.c_str(), "wb");
#endif
    if (!fp)
    {
        return HttpRequest::REQUEST_OPENFILE_ERROR;
    }

    int down_code = HttpRequest::REQUEST_PERFORM_ERROR;

    size_t uCaledCount = _splitDownloadChunks(dTotalSize);
    size_t uExpectedCount = request->getExpectedChunkCount();
    size_t uActualChunkCount = uCaledCount > uExpectedCount ? uExpectedCount : uCaledCount;
    
    // if multi thread download
    if (response.isSupportMultiChunk() && uActualChunkCount > 1)
    {
        long gap = static_cast<long>(dTotalSize) / uActualChunkCount;
#ifdef _WIN32
        std::vector<HANDLE> threads;
#else
        std::vector<pthread_t> threads;
#endif

        for (size_t i = 0; i < uActualChunkCount; i++)
        {
            ThreadChunk* thread_chunk = new ThreadChunk;
            thread_chunk->_fp = fp;
            thread_chunk->_download = this;

            if (i < uActualChunkCount - 1)
            {
                thread_chunk->_startidx = i * gap;
                thread_chunk->_endidx = thread_chunk->_startidx + gap - 1;
            }
            else
            {
                thread_chunk->_startidx = i * gap;
                thread_chunk->_endidx = -1;
            }

#ifdef _WIN32
            DWORD thread_id;
            HANDLE hThread = CreateThread(NULL, 0, HttpHelper::DownloadWork, thread_chunk, 0, &(thread_id));
#else
            pthread_t hThread;
            pthread_create(&hThread, NULL, HttpHelper::DownloadWork, thread_chunk);
#endif
            threads.push_back(hThread);
        }

#ifdef _WIN32
        WaitForMultipleObjects(threads.size(), &threads[0], TRUE, INFINITE);
        for (HANDLE handle : threads)
        {
            CloseHandle(handle);
        }
#else
        for (pthread_t thread : threads)
        {
            pthread_join(thread, NULL);
        }
#endif
    }
    else
    {
        ThreadChunk* thread_chunk = new ThreadChunk;
        thread_chunk->_fp = fp;
        thread_chunk->_download = this;
        thread_chunk->_startidx = 0;
        thread_chunk->_endidx = 0;
        down_code = DoDownload(thread_chunk);
    }

    fclose(fp);

    if (m_download_fail == false)
    {
#ifdef _WIN32
        MoveFileExA(out_file_name.c_str(), src_file_name.c_str(), MOVEFILE_REPLACE_EXISTING);
#else
        unlink(src_file_name.c_str());
        rename(out_file_name.c_str(), src_file_name.c_str());
#endif
    }
    else
    {
#ifdef _WIN32
        DeleteFileA(out_file_name.c_str());
#else
        unlink(out_file_name.c_str());
#endif
    }

    m_result_callback(m_id, m_download_fail ? false : true, m_error_string);

    m_is_running = false;

    return down_code;
}
//-----------------------------------------------------------------------
double HttpDownloadTaskLoop::_getDownloadFileSize(HttpDownloadRequest* request)
{
    HttpDownloadRequest::HttpDownloadResponse& response = request->response();
    if (request->getUrl().empty())
    {
        return -1.0;
    }
    else
    {
        double dFileLength = -1.0;
        CURL *handle = curl_easy_init();
        HttpHelper::set_share_handle(handle);

        if (handle)
        {
            std::string     szReceiveHeader;

            curl_easy_setopt(handle, CURLOPT_URL, request->getUrl().c_str());
            curl_easy_setopt(handle, CURLOPT_HEADER, 1);
            curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
            curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 5);
            curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, receiveHeaderFunction);
            curl_easy_setopt(handle, CURLOPT_HEADERDATA, &szReceiveHeader);
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, receiveBodyFunction);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
            curl_easy_setopt(handle, CURLOPT_RANGE, "2-");

            CURLcode curl_code = curl_easy_perform(handle);

            // retry
            if (curl_code == CURLE_OPERATION_TIMEDOUT)
            {
                size_t retry_count = request->getRetry();
                while (retry_count > 0)
                {
                    curl_code = curl_easy_perform(handle);
                    if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
                    retry_count--;
                }
            }

            long lResultCode = 0L;
            curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &lResultCode);

            if (curl_code == CURLE_OK)
            {
                curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &dFileLength);
                response.setFileLength(dFileLength);

                // if match "Content-Range: bytes 2-1449/26620", means server support multi thread download
                std::regex pattern("CONTENT-RANGE\\s*:\\s*\\w+\\s*(\\d+)-(\\d*)/(\\d+)", std::regex::icase);
                bool bSupport = std::regex_search(szReceiveHeader, pattern);
                response.setSupportMultiChunk(bSupport);
            }
            else
            {
                const char* err_string = curl_easy_strerror(curl_code);
                response.setErrorBuffer(err_string);
            }

            curl_easy_cleanup(handle);
        }

        return dFileLength;
    }
}
//-----------------------------------------------------------------------
int HttpDownloadTaskLoop::_doDownload(HttpDownloadRequest::HttpDownloadResponse::Chunk* chunk)
{
    HttpDownloadRequest* request = chunk->request;
    HttpDownloadRequest::HttpDownloadResponse& response = request->response();

    CURL* curl_handle = curl_easy_init();
    HttpHelper::set_share_handle(curl_handle);

    
    if (request->getUrl().substr(0, 5) == "https")
    {
        std::string sslCaFilename = getSSLVerification();
        if (sslCaFilename.empty())
        {
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
        }
        else
        {
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 2L);
            curl_easy_setopt(curl_handle, CURLOPT_CAINFO, sslCaFilename.c_str());
        }
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, request->getUrl().c_str());

//     const char* user_agent = ("Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0");
//     curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, user_agent);

    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 0L);

    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, getTimeoutForRead());   //0 means block always

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, downloadCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, chunk);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, receiveHeaderFunction);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, NULL);

    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, progressCallback);
    curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, chunk);

    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 5L);

    if (chunk->end != 0)
    {
        StringStream ss;
        if (chunk->end > 0)
        {
            ss << chunk->start << "-" << chunk->end;
        }
        else
        {
            ss << chunk->start << "-";
        }

        curl_easy_setopt(curl_handle, CURLOPT_RANGE, ss.str().c_str());
    }

    CURLcode curl_code = curl_easy_perform(curl_handle);
    if (curl_code == CURLE_OPERATION_TIMEDOUT)
    {
        size_t uRetryCount = request->getRetry();
        while (uRetryCount > 0)
        {
            curl_code = curl_easy_perform(curl_handle);
            if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
            uRetryCount--;
        }
    }

    long lResultCode;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &lResultCode);
    if (curl_code == CURLE_OK && (lResultCode >= 200 && lResultCode <= 300))
    {
        if (response.getResultCode() == 0)
        {
            response.setResultCode(lResultCode);
        }
        thread_chunk->_download->m_download_fail = false;
    }
    else
    {
        const char* err_string = curl_easy_strerror(curl_code);
        response.setErrorBuffer(err_string);
        thread_chunk->_download->m_download_fail = true;
        if (response.getResultCode() == 0 
            || (response.getResultCode() >= 200 && response.getResultCode() <= 300))
        {
            response.setResultCode(lResultCode);
        }
    }

    curl_easy_cleanup(curl_handle);

    delete thread_chunk;

    return curl_code;
}
//-----------------------------------------------------------------------
size_t HttpDownloadTaskLoop::_splitDownloadChunks(u2uint64 totalSize)
{
    const u2uint64 size_2mb = 2 * 1024 * 1024;
    const u2uint64 size_10mb = 10 * 1024 * 1024;
    const u2uint64 size_50mb = 50 * 1024 * 1024;

    if (totalSize <= size_2mb)
    {
        return 1;
    }
    else if (totalSize > size_2mb && totalSize <= size_10mb)
    {
        return static_cast<size_t>(totalSize / (size_2mb));
    }
    else if (totalSize > size_10mb && totalSize <= size_50mb)
    {
        return HttpDownloadRequest::DefaultChunkCount;
    }
    else
    {
        size_t chunkCount = static_cast<size_t>(totalSize / size_10mb);
        return chunkCount > 10 ? 10 : chunkCount;
    }

    return 1;
}