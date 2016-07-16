#ifndef __U2HttpDownloadClient_H__
#define __U2HttpDownloadClient_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "threading/U2ThreadHeaders.h"
#include "U2TaskLoop.h"
#include "U2Task.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class Chunk;
class OutStream;

class _U2Export HttpDownloadRequest : public Task
{
public:
    HttpDownloadRequest(const String& type, const String& name);
    virtual ~HttpDownloadRequest();

    void setUrl(const String& url) { m_szUrl = url; };
    const String& getUrl() const { return m_szUrl; };

    void setRetry(size_t retry) { m_nRetry = retry; };
    size_t getRetry() const { return m_nRetry; };

    void setFile(const String& file) { m_szFile = file; };
    const String& getFile() const { return m_szFile; };

    /** Set custom-defined headers.
    @param pHeaders the string vector of custom-defined headers.
    */
    void setHttpHeaders(const vector<String>::type& headers)
    {
        m_Headers = headers;
    }

    /** Get custom headers.
    @return std::vector<std::string> the string vector of custom-defined headers.
    */
    const vector<String>::type& getHttpHeaders() const
    {
        return m_Headers;
    }

    void setExpectedChunkCount(size_t n) { m_nExpectedChunkCount = n; };
    size_t getExpectedChunkCount() { return m_nExpectedChunkCount; };
    
    virtual void run() override;

    void setTotalFileLength(u2uint64 len) { m_ulTotalFileLen = len; };
    u2uint64 getTotalFileLength() { return m_ulTotalFileLen; };

    void setChunked(bool chunked);
    bool isChunked() const;

    Chunk* createChunk(u2uint64 start, u2uint64 end);
    Chunk* retrieveChunk(const String& guid);
    void destroyChunk(const String& guid);
    Chunk* retrieveNextWaitingChunk();

    void _createStream();
    size_t _writeStream(size_t start, size_t size, const u2byte* data);

public:
    static const size_t DefaultChunkCount = 5;

protected:
    String		m_szUrl;
    String		m_szFile;
    size_t      m_nRetry;
    size_t      m_nExpectedChunkCount;
    vector<String>::type	m_Headers;
    u2uint64    m_ulTotalFileLen;

    U2_MUTEX(m_ChunksMtx);
    typedef map<String, Chunk*>::type ChunkMap;
    ChunkMap    m_ChunkMap;
    bool        m_bChunked;

    U2_MUTEX(m_OutMtx);
    OutStream*  m_out;
};


class _U2Export Chunk : public Object
{
public:
    enum eDownloadState
    {
        HDPS_Unkown,
        HDPS_Waiting,
        HDPS_Downloading,
        HDPS_Download_Fail,
        HDPS_Download_Succeed
    };

public:
    Chunk(const String& type, const String& name);
    virtual ~Chunk();

    void initialize(HttpDownloadRequest* request, u2uint64 start, u2uint64 end);

    void setDownloadState(eDownloadState state) { m_eDownloadState = state; };
    eDownloadState getDownloadState() const { return m_eDownloadState; };

    inline u2uint64 getStart() const;
    inline u2uint64 getEnd() const;
    inline u2uint64 getTotal() const;
    inline u2uint64 getDownloaded() const;

    const String& getUrl() const;
    size_t getRetry() const;

    /** Set the http response code.
    @param value the http response code that represent whether the request is successful or not.
    */
    inline void setResultCode(long value);

    /** Get the http response code to judge whether response is successful or not.
    I know that you want to see the _responseCode is 200.
    If _responseCode is not 200, you should check the meaning for _responseCode by the net.
    @return long the value of _responseCode
    */
    inline long getResultCode() const;

    inline void setErrorBuffer(const char* value);

    /** Get the error buffer which will tell you more about the reason why http request failed.
    @return const char* the pointer that point to _errorBuffer.
    */
    inline const String& getErrorBuffer() const;

    void _createStream();
    size_t _writeStream(const u2byte* data, size_t size);

protected:
    u2uint64    m_ulStart;
    u2uint64    m_ulEnd;
    u2uint64    m_ulTotal;
    u2uint64    m_ulDownloaded;
    HttpDownloadRequest*    m_pRequest;
    eDownloadState          m_eDownloadState;
    long        m_lResultCode;   //< the status code returned from libcurl, e.g. 200, 404
    String      m_szErrorBuffer;
};


class _U2Export HttpDownloadTaskLoop : public TaskLoop
{
public:
    explicit HttpDownloadTaskLoop(const String& type, const String& name);
    virtual ~HttpDownloadTaskLoop();


    virtual void postTask(Task* task) override;

    virtual void run() override;

    virtual void quit() override;

    virtual void pause() override;

    virtual void resume() override;

    virtual String getThreadId() override;

    /**
    * Set the timeout value for connecting.
    *
    * @param value the timeout value for connecting.
    */
    void setTimeoutForConnect(size_t value);

    /**
    * Get the timeout value for connecting.
    *
    * @return int the timeout value for connecting.
    */
    size_t getTimeoutForConnect();

    /**
    * Set the timeout value for reading.
    *
    * @param value the timeout value for reading.
    */
    void setTimeoutForRead(size_t value);

    /**
    * Get the timeout value for reading.
    *
    * @return int the timeout value for reading.
    */
    size_t getTimeoutForRead();

    /**
    * Set root certificate path for SSL verification.
    *
    * @param caFile a full path of root certificate.if it is empty, SSL verification is disabled.
    */
    void setSSLVerification(const String& caFile);

    /**
    * Get ths ssl CA filename
    *
    * @return the ssl CA filename
    */
    const String& getSSLVerification();

    /**
    * Enable cookie support.
    *
    * @param cookieFile the filepath of cookie file.
    */
    void enableCookies(const String& cookieFile);

    /**
    * Get the cookie filename
    *
    * @return the cookie filename
    */
    const String& getCookieFilename();

    inline void setTotalThreadCount(size_t count);
    inline size_t getTotalThreadCount();

    inline void setMaxThreadCountPerRequest(size_t count);
    inline size_t getMaxThreadCountPerRequest();
    

protected:
    virtual void postTaskAndReply(Task* task, Task* reply) override;

    virtual void _runInternal() = 0;

    virtual void _addToIncomingQueue(Task* task);

    Chunk* _getNextWaitingChunk();

    u2uint64 _getFileLength(HttpDownloadRequest* request, String& header);
    int _doDownloadChunk(Chunk* chunk);

    size_t _splitDownloadChunks(u2uint64 totalSize);

protected:
    std::thread     m_thread;

    // This flag is set to false when Run should return.
    U2_MUTEX(m_KeepRunningMutex);
    bool m_bKeepRunning;

    U2_MUTEX(m_PausingMutex);
    bool m_bPausing;

    U2_MUTEX(m_TimeoutForConnectMutex);
    size_t m_uTimeoutForConnect;

    U2_MUTEX(m_TimeoutForReadMutex);
    size_t m_uTimeoutForRead;

    U2_MUTEX(m_SslCaFilenameMutex);
    String m_szSslCaFilename;

    U2_MUTEX(m_CookieFilenameMutex);
    String m_szCookieFilename;

    U2_MUTEX(m_TotalThreadCountMutex);
    size_t m_uTotalThreadCount;

    U2_MUTEX(m_MaxThreadCountPerRequestMutex);
    size_t m_uMaxThreadCountPerRequest;

    U2_MUTEX(m_mtxIncomingQueue);
    U2_THREAD_SYNCHRONISER(m_IncomingQueueSync);
    list<Task*>::type m_IncomingQueue;

    typedef multimap<HttpDownloadRequest*, Chunk*>::type    ChunkMultiMap;
    ChunkMultiMap m_Chunks;

    typedef map<std::thread::id, std::thread>::type     ThreadMap;
    ThreadMap m_ThreadMap;
};



#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2HttpDownloadClient_H__
