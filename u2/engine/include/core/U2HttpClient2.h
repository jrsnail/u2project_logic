#ifndef __U2HttpClient2_H__
#define __U2HttpClient2_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "threading/U2ThreadHeaders.h"
#include "U2Singleton.h"
#include "U2Task.h"
#include "U2TaskLoop.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class _U2Export HttpRequest : public Task
{
public:
	/**
	* The HttpProtocol type enum.
	*/
	enum class Type
	{
		HTTP_GET,
		HTTP_POST,
		HTTP_PUT,
		HTTP_DELETE,
		HTTP_UNKNOWN,
	};

public:
    HttpRequest(const String& type, const String& name)
		: Task(type, name)
	{}
	virtual ~HttpRequest() {};

// 	virtual void preRecycleByPool() override;
// 	virtual void postReuseFromPool() override;

	virtual bool canConcate(const HttpRequest* request);

	void setUrl(const String& url) { m_szUrl = url; };
	const String& getUrl() const { return m_szUrl; };

	void setHttpType(const HttpRequest::Type& type) { m_eType = type; };
	const HttpRequest::Type getHttpType() const { return m_eType; };

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

    /** Get the request data pointer of HttpRequest object.
    @return char* the request data pointer.
    */
    inline const u2char* const getData();

    /** Get the size of request data
    @return ssize_t the size of request data
    */
    inline size_t getDataSize();


    virtual void run() override;

protected:
	String		m_szUrl;
	Type		m_eType;
	vector<String>::type	m_Headers;
    vector<u2char>::type	m_Data;
};


class _U2Export HttpResponse : public Task
{
public:
    HttpResponse(const String& type, const String& name);
    virtual ~HttpResponse();

//     virtual void preRecycleByPool() override;
//     virtual void postReuseFromPool() override;

    /** Set response headers.
    @param pHeaders the string vector of response headers.
    */
    inline void setHttpHeader(const vector<u2char>::type& data)
    {
        m_Headers = data;
    }

    /** Get response headers.
    @return std::vector<std::string> the string vector of response headers.
    */
    inline vector<u2char>::type* getHttpHeader()
    {
        return &m_Headers;
    }

    /** Set the http response code.
    @param value the http response code that represent whether the request is successful or not.
    */
    inline void setResultCode(long value)
    {
        m_lResultCode = value;
    }

    /** Get the http response code to judge whether response is successful or not.
    I know that you want to see the _responseCode is 200.
    If _responseCode is not 200, you should check the meaning for _responseCode by the net.
    @return long the value of _responseCode
    */
    inline u2int64 getResultCode()
    {
        return m_lResultCode;
    }

    /** Set the error buffer which will tell you more the reason why http request failed.
    @param value a string pointer that point to the reason.
    */
    inline void setErrorBuffer(const char* value)
    {
        m_szErrorBuffer.clear();
        m_szErrorBuffer.assign(value);
    }

    /** Get the error buffer which will tell you more about the reason why http request failed.
    @return const char* the pointer that point to _errorBuffer.
    */
    inline const String& getErrorBuffer()
    {
        return m_szErrorBuffer;
    }

    /** Get the request data pointer of HttpRequest object.
    @return char* the request data pointer.
    */
    inline u2char* getData();

    /** Get the size of request data
    @return ssize_t the size of request data
    */
    inline size_t getDataSize();

    inline void setSucceed(bool succeed) { m_bSucceed = succeed; };

    inline bool isSucceed() const { return m_bSucceed; };

    virtual void run() override;

protected:
    vector<u2char>::type	m_Headers;
    vector<u2char>::type	m_Data;
    u2int64                 m_lResultCode;   //< the status code returned from libcurl, e.g. 200, 404
    String					m_szErrorBuffer;   //< if _responseCode != 200, please read _errorBuffer to find the reason
    bool                    m_bSucceed;
};


class _U2Export HttpTaskLoop : public TaskLoop
{
public:
    explicit HttpTaskLoop(const String& type, const String& name);
    virtual ~HttpTaskLoop();

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

    void processTask(HttpRequest* request, char* responseMessage);

protected:
    virtual void postTaskAndReply(Task* task, Task* reply) override;

    void _runInternal();

    void _addToIncomingQueue(Task* task);

protected:
    std::thread     m_thread;

    // Protect access to m_IncomingQueue.
    U2_MUTEX(m_mtxIncomingQueue);
    U2_THREAD_SYNCHRONISER(m_IncomingQueueSync);
    // A null terminated list which creates an incoming_queue of tasks that are
    // acquired under a mutex for processing on this instance's thread. These
    // tasks have not yet been sorted out into items for our work_queue_ vs items
    // that will be handled by the TimerManager.
    std::queue<Task*> m_IncomingQueue;

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


#endif // __U2TypedObjectManager_H__
