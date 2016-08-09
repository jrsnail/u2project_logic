#include "U2HttpDownloadClient.h"

#include <curl/curl.h>
#include <regex>
#include "U2Exception.h"
#include "U2LogManager.h"
#include "U2FileSystemLayer.h"
#include "U2DataPool.h"
#include "cocos2d.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
String HttpDownloadRequest::ExtName = ".udl";
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpDownloadRequest::HttpDownloadRequest(const String& type, const String& name, const u2::String& guid)
    : Task(type, name, guid)
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
void HttpDownloadRequest::setFile(const String& file)
{
    if (m_szFile != BLANK)
    {
        assert(0);
    }
    m_szFile = file;
    m_szTempFile = file + ExtName;
}
//-----------------------------------------------------------------------
void HttpDownloadRequest::setErrorBuffer(const char* value)
{
    m_szErrorBuffer.clear();
    m_szErrorBuffer.assign(value);
}
//-----------------------------------------------------------------------
const String& HttpDownloadRequest::getErrorBuffer() const
{
    return m_szErrorBuffer;
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
        U2_LOCK_MUTEX(m_ChunksMtx);
        m_ChunkMap[pObj->getGuid()] = pObj;
    }
    return pObj;
}
//-----------------------------------------------------------------------
Chunk* HttpDownloadRequest::retrieveChunk(const String& guid)
{
    U2_LOCK_MUTEX(m_ChunksMtx);
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
    U2_LOCK_MUTEX(m_ChunksMtx);
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
    U2_LOCK_MUTEX(m_ChunksMtx);
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
bool HttpDownloadRequest::isAllChunksSucceed()
{
    U2_LOCK_MUTEX(m_ChunksMtx);
    bool bAllSucceed = true;
    for (ChunkMap::iterator it = m_ChunkMap.begin();
    it != m_ChunkMap.end(); it++)
    {
        if (it->second->getDownloadState() != Chunk::eDownloadState::HDPS_Download_Succeed)
        {
            bAllSucceed = false;
            break;
        }
    }
    return bAllSucceed;
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
void HttpDownloadRequest::_createStream()
{
    assert(isChunked());

    if (m_pFileHandle != nullptr)
    {
        return;
    }

    U2_LOCK_MUTEX(m_OutMtx);

    // check file existed
    bool bExisted = FileSystemLayer::fileExists(getTempFile());
    // if existed
    if (bExisted)
    {
        m_pFileHandle = fopen(getTempFile().c_str(), "ab");
    }
    // if not, then create it and fill with 0
    else
    {
        m_pFileHandle = fopen(getTempFile().c_str(), "ab");

        const size_t DataSize = 1024;
        u2ubyte data[DataSize];
        memset(data, 0, DataSize);

        size_t uWriteCount = 0;
        while (uWriteCount < m_ulTotalFileLen)
        {
            size_t uDelta = m_ulTotalFileLen - uWriteCount;
            uWriteCount += fwrite(data, 1, uDelta > DataSize ? DataSize : uDelta, m_pFileHandle);
        }
    }

    if (m_pFileHandle == nullptr)
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
size_t HttpDownloadRequest::_writeStream(size_t start, size_t size, const u2byte* data)
{
    if (m_pFileHandle == nullptr)
    {
        assert(0);
    }
    U2_LOCK_MUTEX(m_OutMtx);
    fseek(m_pFileHandle, static_cast<long>(start), SEEK_SET);
    size_t uWriteCount = 0;
    while (uWriteCount < size)
    {
        uWriteCount += fwrite(data + uWriteCount, 1, size - uWriteCount, m_pFileHandle);
    }
    return uWriteCount;
}
//-----------------------------------------------------------------------
void HttpDownloadRequest::serialize()
{
    U2_LOCK_MUTEX(m_ChunksMtx);
}
//-----------------------------------------------------------------------
void HttpDownloadRequest::deserialize()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Chunk::Chunk(const String& type, const String& name, const u2::String& guid)
    : Object(type, name, guid)
    , m_ulStart(0L)
    , m_ulEnd(0L)
    , m_ulTotal(0L)
    , m_ulDownloaded(0L)
    , m_pRequest(nullptr)
    , m_eDownloadState(HDPS_Unkown)
    , m_lResultCode(0)
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
    m_ulTotal = m_ulEnd - m_ulStart;
    m_ulDownloaded = 0;
}
//-----------------------------------------------------------------------
u2uint64 Chunk::getStart() const
{
    return m_ulStart;
}
//-----------------------------------------------------------------------
u2uint64 Chunk::getEnd() const
{
    return m_ulEnd;
}
//-----------------------------------------------------------------------
u2uint64 Chunk::getTotal() const
{
    return m_ulTotal;
}
//-----------------------------------------------------------------------
u2uint64 Chunk::getDownloaded() const
{
    return m_ulDownloaded;
}
//-----------------------------------------------------------------------
const String& Chunk::getUrl() const
{
    if (m_pRequest == nullptr)
    {
        assert(0);
    }
    return m_pRequest->getUrl();
}
//-----------------------------------------------------------------------
size_t Chunk::getRetry() const
{
    if (m_pRequest == nullptr)
    {
        assert(0);
    }
    return m_pRequest->getRetry();
}
//-----------------------------------------------------------------------
const String& Chunk::getFile() const
{
    if (m_pRequest == nullptr)
    {
        assert(0);
    }
    return m_pRequest->getFile();
}
//-----------------------------------------------------------------------
const String& Chunk::getTempFile() const
{
    if (m_pRequest == nullptr)
    {
        assert(0);
    }
    return m_pRequest->getTempFile();
}
//-----------------------------------------------------------------------
void Chunk::setResultCode(long value)
{
    m_lResultCode = value;
}
//-----------------------------------------------------------------------
long Chunk::getResultCode() const
{
    return m_lResultCode;
}
//-----------------------------------------------------------------------
void Chunk::setErrorBuffer(const char* value)
{
    m_szErrorBuffer.clear();
    m_szErrorBuffer.assign(value);
}
//-----------------------------------------------------------------------
const String& Chunk::getErrorBuffer() const
{
    return m_szErrorBuffer;
}
//-----------------------------------------------------------------------
bool Chunk::isAllChunksSucceed()
{
    if (m_pRequest == nullptr)
    {
        assert(0);
    }
    return m_pRequest->isAllChunksSucceed();
}
//-----------------------------------------------------------------------
void Chunk::_createStream()
{
    if (m_pRequest == nullptr)
    {
        assert(0);
    }
    m_pRequest->_createStream();
}
//-----------------------------------------------------------------------
size_t Chunk::_writeStream(const u2byte* data, size_t size)
{
    if (m_pRequest == nullptr)
    {
        assert(0);
    }
    if (m_ulStart + size > m_ulEnd)
    {
        assert(0);
    }
    size_t uWritten = m_pRequest->_writeStream(m_ulStart, size, data);
    m_ulStart += uWritten;
    m_ulDownloaded += uWritten;
    return uWritten;
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
    Chunk* chunk = reinterpret_cast<Chunk*>(userdata);
    return chunk->_writeStream((const u2byte*)ptr, size * nmemb);
}
//-----------------------------------------------------------------------
static int progressCallback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    Chunk* thread_chunk = reinterpret_cast<Chunk*>(clientp);

    return 0;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpDownloadTaskLoop::HttpDownloadTaskLoop(const String& type, const String& name, const u2::String& guid)
    : TaskLoop(type, name, guid)
    , m_bKeepRunning(true)
    , m_bPausing(false)
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
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    m_bKeepRunning = true;

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
    U2_LOCK_MUTEX(m_KeepRunningMutex);
    m_bKeepRunning = false;

    TaskLoop::quit();
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::pause()
{
    _saveAllRequests();

    U2_LOCK_MUTEX(m_PausingMutex);
    m_bPausing = true;
    TaskLoop::pause();
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::resume()
{
    U2_LOCK_MUTEX(m_PausingMutex);
    m_bPausing = false;
    TaskLoop::resume();
}
//-----------------------------------------------------------------------
String HttpDownloadTaskLoop::getThreadId()
{
    StringStream stream;
    stream << std::this_thread::get_id();
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
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::_runInternal()
{
    for (;;)
    {
        {
            U2_LOCK_MUTEX(m_KeepRunningMutex);
            if (!m_bKeepRunning)
                break;
        }

        {
            U2_LOCK_MUTEX(m_PausingMutex);
            if (m_bPausing)
            {
                U2_THREAD_SLEEP(1000);
                continue;
            }
        }

        Chunk* pChunk = _getNextWaitingChunk();
        if (pChunk == nullptr)
        {
            U2_THREAD_SLEEP(1000);
            continue;
        }
        else
        {
            pChunk->_createStream();
            _doDownloadChunk(pChunk);
            if (pChunk->isAllChunksSucceed())
            {
                _onRequestDownloadSucceed(pChunk->request());
            }
            else
            {
                _saveRequest(pChunk->request());
            }
        }
    }
}
//-----------------------------------------------------------------------
Chunk* HttpDownloadTaskLoop::_getNextWaitingChunk()
{
    HttpDownloadRequest* request = nullptr;

    // find first waiting chunk/request
    {
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
    }

    if (request != nullptr)
    {
        // get file total length
        String szRespHeader;
        u2uint64 ulTotalLen = _getFileLength(request, szRespHeader);
        if (ulTotalLen < 0)
        {
            assert(0);
            return nullptr;
        }
        request->setTotalFileLength(ulTotalLen);

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
    if (request->getUrl().empty())
    {
        return -1.0L;
    }
    else
    {
        double dFileLength = -1.0;
        CURL *handle = curl_easy_init();
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
            }
            else
            {
                const char* err_string = curl_easy_strerror(curl_code);
                request->setErrorBuffer(err_string);
            }

            curl_easy_cleanup(handle);
        }
        return dFileLength;
    }
}
//-----------------------------------------------------------------------
int HttpDownloadTaskLoop::_doDownloadChunk(Chunk* chunk)
{
    CURL* curl_handle = curl_easy_init();

    if (chunk->getUrl().substr(0, 5) == "https")
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

    curl_easy_setopt(curl_handle, CURLOPT_URL, chunk->getUrl().c_str());

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

    if (chunk->getEnd() != 0)
    {
        StringStream ss;
        if (chunk->getEnd() > 0)
        {
            ss << chunk->getStart() << "-" << chunk->getEnd();
        }
        else
        {
            ss << chunk->getStart() << "-";
        }

        curl_easy_setopt(curl_handle, CURLOPT_RANGE, ss.str().c_str());
    }

    CURLcode curl_code = curl_easy_perform(curl_handle);
    if (curl_code == CURLE_OPERATION_TIMEDOUT)
    {
        size_t uRetryCount = chunk->getRetry();
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
        chunk->setResultCode(lResultCode);
        chunk->setDownloadState(Chunk::HDPS_Download_Succeed);
    }
    else
    {
        const char* err_string = curl_easy_strerror(curl_code);
        chunk->setErrorBuffer(err_string);
        chunk->setDownloadState(Chunk::HDPS_Download_Fail);
        chunk->setResultCode(lResultCode);
    }

    curl_easy_cleanup(curl_handle);

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
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::_onRequestDownloadSucceed(HttpDownloadRequest* request)
{
    // rename
    FileSystemLayer::renameFile(request->getTempFile(), request->getFile());

    // decompress

    // delete db record
    DataPool* pPool = DataPoolManager::getSingleton().retrieveObjectByName(getName());
    if (pPool)
    {
        pPool->removeData(request->getFile());
    }

    // remove form incoming queue
    {
        U2_LOCK_MUTEX(m_mtxIncomingQueue);
        for (list<Task*>::iterator it = m_IncomingQueue.begin(); it != m_IncomingQueue.end(); it++)
        {
            if (*it == request)
            {
                m_IncomingQueue.erase(it);
                break;
            }
        }
    }
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::_saveRequest(HttpDownloadRequest* request)
{
    assert(request);
    DataPool* pPool = DataPoolManager::getSingleton().retrieveObjectByName(getName());
    if (pPool)
    {
        request->serialize();
        pPool->saveCharVectorData(request->getFile(), request->getData());
    }
}
//-----------------------------------------------------------------------
void HttpDownloadTaskLoop::_saveAllRequests()
{
    U2_LOCK_MUTEX(m_mtxIncomingQueue);
    for (list<Task*>::iterator it = m_IncomingQueue.begin(); it != m_IncomingQueue.end(); it++)
    {
        HttpDownloadRequest* req = dynamic_cast<HttpDownloadRequest*>(*it);
        _saveRequest(req);
    }
}


/*
#ifdef _WIN32
MoveFileExA(out_file_name.c_str(), src_file_name.c_str(), MOVEFILE_REPLACE_EXISTING);
#else
unlink(src_file_name.c_str());
rename(out_file_name.c_str(), src_file_name.c_str());
#endif
*/