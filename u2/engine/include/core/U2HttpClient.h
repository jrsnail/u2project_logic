#ifndef __U2HttpClient_H__
#define __U2HttpClient_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "threading/U2ThreadHeaders.h"
#include "U2Singleton.h"
#include "U2NetMsg.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class HttpOutMsg : public OutMsg
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
	HttpOutMsg(const String& type)
		: OutMsg(type)
	{}
	virtual ~HttpOutMsg() {};

	virtual void preRecycleByPool() override;
	virtual void postReuseFromPool() override;

	virtual bool canConcate(const OutMsg* msg) override;

	void setUrl(const String& url) { m_szUrl = url; };
	const String& getUrl() const { return m_szUrl; };

	void setHttpType(const HttpOutMsg::Type& type) { m_eType = type; };
	const HttpOutMsg::Type getHttpType() const { return m_eType; };

	/** Set custom-defined headers.
	@param pHeaders the string vector of custom-defined headers.
	*/
	void setOutHeaders(const vector<String>::type& headers)
	{
		m_Headers = headers;
	}

	/** Get custom headers.
	@return std::vector<std::string> the string vector of custom-defined headers.
	*/
	const vector<String>::type& getOutHeaders() const
	{
		return m_Headers;
	}

protected:
	String		m_szUrl;
	Type		m_eType;
	vector<String>::type	m_Headers;
};


class HttpInMsg : public InMsg
{
public:
	HttpInMsg(const String& type);
	virtual ~HttpInMsg();

	virtual void preRecycleByPool() override;
	virtual void postReuseFromPool() override;

	/** Set response headers.
	@param pHeaders the string vector of response headers.
	*/
	inline void setInHeader(const vector<u2char>::type& data)
	{
		m_Headers = data;
	}

	/** Get response headers.
	@return std::vector<std::string> the string vector of response headers.
	*/
	inline vector<u2char>::type* getInHeader()
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

protected:
	vector<u2char>::type	m_Headers;
	u2int64                 m_lResultCode;   //< the status code returned from libcurl, e.g. 200, 404
	String					m_szErrorBuffer;   //< if _responseCode != 200, please read _errorBuffer to find the reason
};


class HttpGroupedOutMsg : public HttpOutMsg
{
public:
	HttpGroupedOutMsg(const String& type);
	virtual ~HttpGroupedOutMsg();

	virtual void preRecycleByPool() override;
	virtual void postReuseFromPool() override;
	virtual String serialize() override;
	virtual void deserialize(const String& str) override;
	bool tryAddOutMsg(const OutMsg* msg);

protected:
	OutMsg*			m_pSample;
	vector<const OutMsg*>::type	m_List;
};


class HttpGroupedInMsg : public HttpInMsg
{
protected:
	typedef vector<HttpInMsg*>::type	InMsgVector;

public:
	HttpGroupedInMsg(const String& type);
	virtual ~HttpGroupedInMsg();

	virtual void preRecycleByPool() override;
	virtual void postReuseFromPool() override;
	virtual String serialize() override;
	virtual void deserialize(const String& str) override;

	typedef ConstVectorIterator<InMsgVector>       ConstInMsgVectorIterator;
	ConstInMsgVectorIterator getInMsgVectorIterator()
	{
		return ConstInMsgVectorIterator(m_List.begin(), m_List.end());
	}

protected:
	InMsgVector			m_List;
};


class HttpMsgCenter : public MsgCenter
{
public:
	HttpMsgCenter(const String& type, const String& name);
	virtual ~HttpMsgCenter();

	virtual void pushOutMsg(OutMsg* msg) override;
	virtual void pushInMsg(Msg* msg) override;
	virtual void onUpdate(float dt) override;

protected:
	U2_MUTEX(m_OrderedOutMsgsMutex);
	list<OutMsg*>::type	m_OrderedOutMsgs;

	U2_MUTEX(m_PredictingOrderedOutMsgsMutex);
	list<OutMsg*>::type	m_PredictedOrderedOutMsgs;

	U2_MUTEX(m_SendingOrderedOutMsgsMutex);
	list<OutMsg*>::type	m_SendingOrderedOutMsgs;

	U2_MUTEX(m_ResponsedOutMsgsMutex);
	list<OutMsg*>::type	m_ResponsedOutMsgs;

	U2_MUTEX(m_UnorderedOutMsgsMutex);
	list<OutMsg*>::type	m_UnorderedOutMsgs;

	HttpClient*				m_pOrderedClient;
};


class HttpClient : public GeneralAllocatedObject, public Singleton < HttpClient >
{
public:
    /** Default constructor.
    */
	HttpClient();

    /** Default destructor.
    */
    virtual ~HttpClient();

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

	bool isIdle();

	void setTask(HttpOutMsg* requestOutMsg);

protected:
	bool _initThread();
	void _networkThread();
	void _processProtocol(HttpProtocol* protocol, char* responseMessage);

public:
	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static HttpClient& getSingleton(void);

	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static HttpClient* getSingletonPtr(void);


protected:
	U2_MUTEX(m_TimeoutForConnectMutex);
	size_t m_uTimeoutForConnect;

	U2_MUTEX(m_TimeoutForReadMutex);
	size_t m_uTimeoutForRead;

	U2_MUTEX(m_SslCaFilenameMutex);
	String m_szSslCaFilename;

	U2_MUTEX(m_CookieFilenameMutex);
	String m_szCookieFilename;

	bool m_bIsInited;

	U2_THREAD_SYNCHRONISER(m_RequestingOutMsgSync);
	U2_MUTEX(m_RequestingOutMsgMutex);
	HttpOutMsg*	m_pRequestingOutMsg;

	/**
	* The buffer size of _responseMessage
	*/
	static const u2int32 RESPONSE_BUFFER_SIZE = 256;
	char m_ResponseMessage[RESPONSE_BUFFER_SIZE];

	HttpOutMsg* m_pRequestSentinel;
};


#include "U2HeaderSuffix.h"

U2EG_NAMESPACE_END


#endif // __U2TypedObjectManager_H__
