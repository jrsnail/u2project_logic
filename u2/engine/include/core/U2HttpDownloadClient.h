#ifndef __U2HttpDownloadClient_H__
#define __U2HttpDownloadClient_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "threading/U2ThreadHeaders.h"
#include "U2TaskLoop.h"
#include "U2HttpClient2.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class _U2Export HttpDownloadRequest : public Task
{
public:
    class HttpDownloadResponse
    {
    public:
        struct Chunk
        {
            long    start;
            long    end;
            HttpDownloadRequest* request;

            Chunk(long start, long end, HttpDownloadRequest* request)
                : start(start)
                , end(end)
                , request(request)
            {

            }
        };

    public:
        HttpDownloadResponse();
        virtual ~HttpDownloadResponse();

        inline void setFileLength(double len);
        inline double getFileLength() const;

        void pushChunk(long start, long end, HttpDownloadRequest* request);
        Chunk& getChunk(size_t i) const;
        inline size_t getChunkCount() const;

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

        inline void setSupportMultiChunk(bool support);
        inline bool isSupportMultiChunk() const;

        inline void setDownloadLength(u2uint64 len);
        inline long getDownloadLength() const;
        inline void appendDownloadLength(u2uint64 delta);

        inline void setCanceled(bool cancel);
        inline bool isCanceled() const;

    protected:
        double m_dFileLength;
        vector<Chunk>::type  m_Chunks;
        long   m_lResultCode;   //< the status code returned from libcurl, e.g. 200, 404
        String m_szErrorBuffer;
        bool   m_bSupportMultiChunk;
        u2uint64   m_ulDownloadLength;
        bool   m_bCanceled;
        
    public:
        FILE* fp;
        U2_MUTEX(m_DownloadMutex);
    };

public:
    HttpDownloadRequest(const String& type, const String& name)
        : Task(type, name)
        , m_nRetry(3)
        , m_nExpectedChunkCount(DefaultChunkCount)
    {}
    virtual ~HttpDownloadRequest() {};

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


    HttpDownloadResponse& response() { return m_response; }

    virtual void run() override;

public:
    static const size_t DefaultChunkCount = 5;

protected:
    String		m_szUrl;
    String		m_szFile;
    size_t      m_nRetry;
    size_t      m_nExpectedChunkCount;
    vector<String>::type	m_Headers;
    HttpDownloadResponse    m_response;
};


class _U2Export HttpDownloadTaskLoop : public TaskLoop
{
public:
    explicit HttpDownloadTaskLoop(const String& type, const String& name);
    virtual ~HttpDownloadTaskLoop();


    virtual void postTask(Task* task) override;

    virtual void run() override;

    virtual void quit() override;

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

    void processTask(HttpDownloadRequest* request, char* responseMessage);

    

protected:
    virtual void postTaskAndReply(Task* task, Task* reply) override;

    virtual void _runInternal() = 0;

    virtual void _addToIncomingQueue(Task* task) = 0;

    double _getDownloadFileSize(HttpDownloadRequest* request);
    int _doDownload(HttpDownloadRequest::HttpDownloadResponse::Chunk* chunk);
    size_t _splitDownloadChunks(double totalSize);

protected:
    std::thread     m_thread;

    // This flag is set to false when Run should return.
    bool m_bKeepRunning;


    U2_MUTEX(m_TimeoutForConnectMutex);
    size_t m_uTimeoutForConnect;

    U2_MUTEX(m_TimeoutForReadMutex);
    size_t m_uTimeoutForRead;

    U2_MUTEX(m_SslCaFilenameMutex);
    String m_szSslCaFilename;

    U2_MUTEX(m_CookieFilenameMutex);
    String m_szCookieFilename;

};



#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2HttpDownloadClient_H__
