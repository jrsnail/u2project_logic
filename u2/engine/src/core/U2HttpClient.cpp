#include "U2HttpClient.h"

#include <curl/curl.h>
#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2NetMsgCenter.h"
#include "U2FrameListenerCollection.h"
#include "U2StreamQueue.h"
#include "U2DataFilterStream.h"
#include "U2StringStream.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void HttpOutMsg::preRecycleByPool()
{
	m_szUrl = BLANK;
	m_eType = HttpOutMsg::Type::HTTP_UNKNOWN;
	m_Headers.clear();

	OutMsg::preRecycleByPool();
}
//-----------------------------------------------------------------------
void HttpOutMsg::postReuseFromPool()
{
	OutMsg::postReuseFromPool();

	m_szUrl = BLANK;
	m_eType = HttpOutMsg::Type::HTTP_UNKNOWN;
	m_Headers.clear();
}
//-----------------------------------------------------------------------
bool HttpOutMsg::canConcate(const OutMsg* msg)
{
	do 
	{
		const HttpOutMsg* pOther = dynamic_cast<const HttpOutMsg*>(msg);
		if (pOther == nullptr)
		{
			break;
		}
		if (this->getMsgType() != MT_PREDICTABLE && this->getMsgType() != MT_SYNCHRONOUS)
		{
			break;
		}
		if (this->getMsgType() != pOther->getMsgType())
		{
			break;
		}
		if (this->getUrl() != pOther->getUrl())
		{
			break;
		}
		if (this->getHttpType() != pOther->getHttpType())
		{
			break;
		}
		if (this->getOutHeaders() != pOther->getOutHeaders())
		{
			break;
		}
		return true;

	} while (false);

	return false;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpInMsg::HttpInMsg(const String& type)
	: InMsg(type)
	, m_lResultCode(0L)
{
}
//-----------------------------------------------------------------------
HttpInMsg::~HttpInMsg()
{
}
//-----------------------------------------------------------------------
void HttpInMsg::preRecycleByPool()
{
	m_Headers.clear();
	m_lResultCode = 0L;
	m_szErrorBuffer = BLANK;

	InMsg::preRecycleByPool();
}
//-----------------------------------------------------------------------
void HttpInMsg::postReuseFromPool()
{
	InMsg::postReuseFromPool();

	m_Headers.clear();
	m_lResultCode = 0L;
	m_szErrorBuffer = BLANK;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpGroupedOutMsg::HttpGroupedOutMsg(const String& type)
	: HttpOutMsg(type)
	, m_pSample(nullptr)
{

}
//-----------------------------------------------------------------------
HttpGroupedOutMsg::~HttpGroupedOutMsg()
{

}
//-----------------------------------------------------------------------
void HttpGroupedOutMsg::preRecycleByPool()
{
	m_pSample = nullptr;
	
	for (vector<const OutMsg*>::const_iterator it = m_List.begin(); 
		it != m_List.end(); 
		it++)
	{
		MsgManager::getSingleton().recycleObject((OutMsg*)(*it));
	}
	m_List.clear();

	HttpOutMsg::preRecycleByPool();
}
//-----------------------------------------------------------------------
void HttpGroupedOutMsg::postReuseFromPool()
{
	HttpOutMsg::postReuseFromPool();

	m_pSample = nullptr;
	m_List.clear();
}
//-----------------------------------------------------------------------
bool HttpGroupedOutMsg::tryAddOutMsg(const OutMsg* msg)
{
	if (m_pSample == nullptr)
	{
		m_List.push_back(msg);

		// set property for grouped http msg
		const HttpOutMsg* pHttpOutMsg = dynamic_cast<const HttpOutMsg*>(msg);
		m_eMsgType = pHttpOutMsg->getMsgType();
		m_szUrl = pHttpOutMsg->getUrl();
		m_eType = pHttpOutMsg->getHttpType();
		m_Headers = pHttpOutMsg->getOutHeaders();

		return true;
	}
	else
	{
		bool bCan = m_pSample->canConcate(msg);
		if (bCan)
		{
			m_List.push_back(msg);
		}
		return bCan;
	}
}
//-----------------------------------------------------------------------
String HttpGroupedOutMsg::serialize()
{
	String str;

	OutStreamQueue<DataFilterOutStream> out;
	out.push<StringOutStream>("aaa", &str, 0);
	out.push<DataFilterOutStream>("bbb");
	out->writeUint32(m_List.size());
	
	// serialize all inner msg
	for (vector<const OutMsg*>::const_iterator it = m_List.begin(); 
		it != m_List.end();
		it++)
	{
		OutMsg* pMsg = const_cast<OutMsg*>(*it);
		out->writeUTFString(pMsg->serialize());
	}

	out->close();

	return str;
}
//-----------------------------------------------------------------------
void HttpGroupedOutMsg::deserialize(const String& str)
{
	assert(m_List.size() == 0);

	InStreamQueue<DataFilterInStream> in;
	in.push<StringInStream>("aaa", &str);
	in.push<DataFilterInStream>("bbb");
	u2uint32 uCount = in->readUint32();

	for (u2uint32 i = 0; i < uCount; i++)
	{
		// parse in-msg type
		HttpOutMsg* pMsg = dynamic_cast<HttpOutMsg*>(MsgManager::getSingleton().reuseObjectAsName(in->readUTFString()));
		assert(pMsg != nullptr);
		// parse in-msg body
		if (pMsg != nullptr)
		{
			pMsg->deserialize(in->readUTFString());
			m_List.push_back(pMsg);
		}
	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpGroupedInMsg::HttpGroupedInMsg(const String& type)
	: HttpInMsg(type)
{

}
//-----------------------------------------------------------------------
HttpGroupedInMsg::~HttpGroupedInMsg()
{

}
//-----------------------------------------------------------------------
void HttpGroupedInMsg::preRecycleByPool()
{
	m_List.clear();

	HttpInMsg::preRecycleByPool();
}
//-----------------------------------------------------------------------
void HttpGroupedInMsg::postReuseFromPool()
{
	HttpInMsg::postReuseFromPool();

	m_List.clear();
}
//-----------------------------------------------------------------------
String HttpGroupedInMsg::serialize()
{
	String str;

	OutStreamQueue<DataFilterOutStream> out;
	out.push<StringOutStream>("aaa", &str, 0);
	out.push<DataFilterOutStream>("bbb");
	out->writeUint32(m_List.size());

	// serialize all inner msg
	for (vector<HttpInMsg*>::const_iterator it = m_List.begin();
		it != m_List.end();
		it++)
	{
		out->writeUTFString((*it)->serialize());
	}

	out->close();

	return str;
}
//-----------------------------------------------------------------------
void HttpGroupedInMsg::deserialize(const String& str)
{
	assert(m_List.size() == 0);

	InStreamQueue<DataFilterInStream> in;
	in.push<StringInStream>("aaa", &str);
	in.push<DataFilterInStream>("bbb");
	u2uint32 uCount = in->readUint32();

	for (u2uint32 i = 0; i < uCount; i++)
	{
		// parse in-msg type
		HttpInMsg* pMsg = dynamic_cast<HttpInMsg*>(MsgManager::getSingleton().reuseObjectAsName(in->readUTFString()));
		assert(pMsg != nullptr);
		// parse in-msg body
		if (pMsg != nullptr)
		{
			pMsg->deserialize(in->readUTFString());
			m_List.push_back(pMsg);
		}
	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HttpMsgCenter::HttpMsgCenter(const String& type, const String& name)
	: MsgCenter(type, name)
	, m_pOrderedClient(nullptr)
{
	FrameListenerCollection::getSingleton().addFrameListener(this, std::bind(&HttpMsgCenter::onUpdate, this, std::placeholders::_1));
}
//-----------------------------------------------------------------------
HttpMsgCenter::~HttpMsgCenter()
{
}
//-----------------------------------------------------------------------
void HttpMsgCenter::onUpdate(float dt)
{
	// process responses
	{
		U2_LOCK_MUTEX(m_ResponsedOutMsgsMutex);
		for (list<OutMsg*>::iterator it = m_ResponsedOutMsgs.begin(); 
			it != m_ResponsedOutMsgs.end(); it++)
		{
			(*it)->execute();

			// recycle out-msg(out & in & predicted)
			MsgManager::getSingleton().recycleObject(*it);
		}
	}

	// process ordered requests
	{
		U2_LOCK_MUTEX(m_OrderedOutMsgsMutex);
		if (m_OrderedOutMsgs.size() > 0)
		{
			if (m_pOrderedClient->isIdle())
			{
				HttpGroupedOutMsg* pGroupedOutMsg = dynamic_cast<HttpGroupedOutMsg*>(
					MsgManager::getSingleton().reuseObjectAsName(GET_OBJECT_TYPE(HttpGroupedOutMsg), BLANK)
					);
				if (pGroupedOutMsg == nullptr)
				{
				}
				else
				{
					// concate several msg as one
					do
					{
						OutMsg* pOutMsg = m_OrderedOutMsgs.front();
						if (pGroupedOutMsg->tryAddOutMsg(pOutMsg))
						{
							U2_LOCK_MUTEX(m_SendingOrderedOutMsgsMutex);
							m_SendingOrderedOutMsgs.push_back(pOutMsg);

							m_OrderedOutMsgs.pop_front();
						}
						else
						{
							break;
						}
					} while (m_OrderedOutMsgs.size() > 0);

					// todo, can here block the main thread?
					m_pOrderedClient->setTask(pGroupedOutMsg);
				}
			}
		}
	}
	
	// process unordered requests
	{
	}
}
//-----------------------------------------------------------------------
void HttpMsgCenter::pushOutMsg(OutMsg* msg)
{
	HttpOutMsg* pHttpOutMsg = dynamic_cast<HttpOutMsg*>(msg);
	switch (pHttpOutMsg->getMsgType())
	{
	case OutMsg::MsgType::MT_SYNCHRONOUS:
	{
		U2_LOCK_MUTEX(m_OrderedOutMsgsMutex);
		m_OrderedOutMsgs.push_back(pHttpOutMsg);
		break;
	}
	case OutMsg::MsgType::MT_ASYNCHRONOUS:
	{
		U2_LOCK_MUTEX(m_UnorderedOutMsgsMutex);
		m_UnorderedOutMsgs.push_back(pHttpOutMsg);
		break;
	}
	case OutMsg::MsgType::MT_PREDICTABLE:
	{
		U2_LOCK_MUTEX(m_OrderedOutMsgsMutex);
		m_OrderedOutMsgs.push_back(pHttpOutMsg);

		// push in predicted queue
		//m_PredictedOrderedOutMsgs.push_back(pHttpOutMsg);
		// immediately send to simulator server
		break;
	}
	default:
		assert(0);
	}
}
//-----------------------------------------------------------------------
void HttpMsgCenter::pushInMsg(Msg* msg)
{
	HttpOutMsg* pHttpOutMsg = dynamic_cast<HttpOutMsg*>(msg);
	if (pHttpOutMsg == nullptr)
	{
		return;
	}
	switch (pHttpOutMsg->getMsgType())
	{
	case OutMsg::MsgType::MT_ASYNCHRONOUS:
		break;
	case OutMsg::MsgType::MT_SYNCHRONOUS:
	case OutMsg::MsgType::MT_PREDICTABLE:
	{
		HttpGroupedInMsg* pHttpGroupedInMsg = dynamic_cast<HttpGroupedInMsg*>(pHttpOutMsg->getInMsg());
		// it's a single-msg
		if (pHttpGroupedInMsg == nullptr)
		{
			HttpInMsg* pHttpInMsg = dynamic_cast<HttpInMsg*>(pHttpOutMsg->getInMsg());

			U2_LOCK_MUTEX(m_SendingOrderedOutMsgsMutex);
			for (list<OutMsg*>::iterator sendingIt = m_SendingOrderedOutMsgs.begin();
				sendingIt != m_SendingOrderedOutMsgs.end();
				sendingIt++)
			{
				// find paired out msg
				if ((*sendingIt)->getName() == pHttpInMsg->getOutMsgName())
				{
					(*sendingIt)->setInMsg(pHttpInMsg);
					// remove from sending queue
					m_SendingOrderedOutMsgs.erase(sendingIt);

					// add to response queue
					U2_LOCK_MUTEX(m_ResponsedOutMsgsMutex);
					m_ResponsedOutMsgs.push_back(*sendingIt);
				}
			}
		}
		// it's a grouped-msg
		else
		{
			// loop every inner-msg
			HttpGroupedInMsg::ConstInMsgVectorIterator it = pHttpGroupedInMsg->getInMsgVectorIterator();
			while (it.hasMoreElements())
			{
				// set properties of inner-msg with grouped-msg properties
				HttpInMsg* pHttpInMsg = it.getNext();
				pHttpInMsg->setResultCode(pHttpGroupedInMsg->getResultCode());
				pHttpInMsg->setErrorBuffer(pHttpGroupedInMsg->getErrorBuffer().c_str());
				pHttpInMsg->setInHeader(*(pHttpGroupedInMsg->getInHeader()));

				U2_LOCK_MUTEX(m_SendingOrderedOutMsgsMutex);
				for (list<OutMsg*>::iterator sendingIt = m_SendingOrderedOutMsgs.begin(); 
					sendingIt != m_SendingOrderedOutMsgs.end(); 
					sendingIt++)
				{
					// find paired out msg
					if ((*sendingIt)->getName() == pHttpInMsg->getOutMsgName())
					{
						(*sendingIt)->setInMsg(pHttpInMsg);
						// remove from sending queue
						m_SendingOrderedOutMsgs.erase(sendingIt);

						// add to response queue
						U2_LOCK_MUTEX(m_ResponsedOutMsgsMutex);
						m_ResponsedOutMsgs.push_back(*sendingIt);
					}
				}
			}

			// recycle grouped-msg(out & in & predicted)
			MsgManager::getSingleton().recycleObject(pHttpOutMsg);
		}
		break;
	}
	default:
		assert(0);
	}

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> HttpClient* Singleton<HttpClient>::msSingleton = 0;
HttpClient* HttpClient::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = U2_NEW HttpClient;
	}
	return msSingleton;
}
HttpClient& HttpClient::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
HttpClient::HttpClient() 
	: m_uTimeoutForConnect(30)
	, m_uTimeoutForRead(60)
	, m_bIsInited(false)
	, m_pRequestingOutMsg(nullptr)
	, m_pRequestSentinel(nullptr)
{
	memset(m_ResponseMessage, 0, RESPONSE_BUFFER_SIZE * sizeof(char));
}
//-----------------------------------------------------------------------
HttpClient::~HttpClient()
{
}
//-----------------------------------------------------------------------
void HttpClient::setTimeoutForConnect(size_t value)
{
	U2_LOCK_MUTEX(m_TimeoutForConnectMutex);
	m_uTimeoutForConnect = value;
}
//-----------------------------------------------------------------------
size_t HttpClient::getTimeoutForConnect()
{
	U2_LOCK_MUTEX(m_TimeoutForConnectMutex);
	return m_uTimeoutForConnect;
}
//-----------------------------------------------------------------------
void HttpClient::setTimeoutForRead(size_t value)
{
	U2_LOCK_MUTEX(m_TimeoutForReadMutex);
	m_uTimeoutForRead = value;
}
//-----------------------------------------------------------------------
size_t HttpClient::getTimeoutForRead()
{
	U2_LOCK_MUTEX(m_TimeoutForReadMutex);
	return m_uTimeoutForRead;
}
//-----------------------------------------------------------------------
void HttpClient::setSSLVerification(const String& caFile)
{
	U2_LOCK_MUTEX(m_SslCaFilenameMutex);
	m_szSslCaFilename = caFile;
}
//-----------------------------------------------------------------------
const String& HttpClient::getSSLVerification()
{
	U2_LOCK_MUTEX(m_SslCaFilenameMutex);
	return m_szSslCaFilename;
}
//-----------------------------------------------------------------------
void HttpClient::enableCookies(const String& cookieFile)
{
	U2_LOCK_MUTEX(m_CookieFilenameMutex);
	if (cookieFile != BLANK)
	{
		m_szCookieFilename = cookieFile;
	}
	else
	{
		m_szCookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
	}
}
//-----------------------------------------------------------------------
const String& HttpClient::getCookieFilename()
{
	U2_LOCK_MUTEX(m_CookieFilenameMutex);
	return m_szCookieFilename;
}
//-----------------------------------------------------------------------
bool HttpClient::isIdle()
{
	U2_LOCK_MUTEX(m_RequestingOutMsgMutex);
	return m_pRequestingOutMsg == nullptr;
}
//-----------------------------------------------------------------------
void HttpClient::setTask(HttpOutMsg* requestOutMsg)
{
	U2_LOCK_MUTEX(m_RequestingOutMsgMutex);
	assert(m_pRequestingOutMsg == nullptr);
	m_pRequestingOutMsg = requestOutMsg;
	U2_THREAD_NOTIFY_ALL(m_RequestingOutMsgSync);
}
//-----------------------------------------------------------------------
bool HttpClient::_initThread()
{
	if (m_bIsInited)
	{
		return true;
	}
	else
	{
		auto t = std::thread(std::bind(HttpClient::_networkThread, this));
		t.detach();
		m_bIsInited = true;
	}

	return true;
}
//-----------------------------------------------------------------------
void HttpClient::_networkThread()
{
	while (true)
	{
		// step 1: send http request if the request isn't empty
		U2_LOCK_MUTEX(m_RequestingOutMsgMutex);
		if (m_pRequestingOutMsg == nullptr)
		{
			U2_THREAD_WAIT(m_RequestingOutMsgSync, lck);
		}

		if (m_pRequestingOutMsg == m_pRequestSentinel)
		{
			break;
		}

		// step 2: libcurl sync access

		// Create a HttpResponse object, the default setting is http access failed
		HttpResponse *response = new (std::nothrow) HttpResponse(request);

		_processProtocol(response, m_ResponseMessage);

		for (size_t i = 0; i < 3; i++)
		{
			HttpOutMsg* request = dynamic_cast<HttpOutMsg*>(protocol->getOutMsg());
			HttpInMsg* response = dynamic_cast<HttpInMsg*>(protocol->getInMsg());
			if (request == nullptr || response == nullptr)
			{
				continue;
			}

			long responseCode = -1;
			int retValue = 0;

			// Process the request -> get response packet
			switch (request->getHttpType())
			{
			case HttpOutMsg::Type::HTTP_GET: // HTTP GET
				retValue = processGetTask(this, request,
					writeData,
					response->getData(),
					&responseCode,
					writeHeaderData,
					response->getInHeader(),
					m_ResponseMessage);
				break;

			case HttpOutMsg::Type::HTTP_POST: // HTTP POST
				retValue = processPostTask(this, request,
					writeData,
					response->getData(),
					&responseCode,
					writeHeaderData,
					response->getInHeader(),
					m_ResponseMessage);
				break;

			case HttpOutMsg::Type::HTTP_PUT:
				retValue = processPutTask(this, request,
					writeData,
					response->getData(),
					&responseCode,
					writeHeaderData,
					response->getInHeader(),
					m_ResponseMessage);
				break;

			case HttpOutMsg::Type::HTTP_DELETE:
				retValue = processDeleteTask(this, request,
					writeData,
					response->getData(),
					&responseCode,
					writeHeaderData,
					response->getInHeader(),
					m_ResponseMessage);
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
				response->setErrorBuffer(m_ResponseMessage);
			}
			else
			{
				response->setSucceed(true);
				MsgCenter::getSingleton().popOutMsg();
				break;
			}
		}


		// add response packet into queue
		_responseQueueMutex.lock();
		_responseQueue.pushBack(response);
		_responseQueueMutex.unlock();

	}

	// cleanup: if worker thread received quit signal, clean up un-completed request queue
	U2_LOCK_MUTEX(m_RequestingOutMsgMutex);
	m_pRequestingOutMsg = nullptr;
}
//-----------------------------------------------------------------------
void HttpClient::_processProtocol(HttpProtocol* protocol, char* responseMessage)
{
	HttpOutMsg* request = dynamic_cast<HttpOutMsg*>(protocol->getOutMsg());
	HttpInMsg* response = dynamic_cast<HttpInMsg*>(protocol->getInMsg());
	if (request == nullptr || response == nullptr)
	{
		return;
	}

	long responseCode = -1;
	int retValue = 0;

	// Process the request -> get response packet
	switch (request->getHttpType())
	{
	case HttpOutMsg::Type::HTTP_GET: // HTTP GET
		retValue = processGetTask(this, request,
			writeData,
			response->getData(),
			&responseCode,
			writeHeaderData,
			response->getInHeader(),
			responseMessage);
		break;

	case HttpOutMsg::Type::HTTP_POST: // HTTP POST
		retValue = processPostTask(this, request,
			writeData,
			response->getData(),
			&responseCode,
			writeHeaderData,
			response->getInHeader(),
			responseMessage);
		break;

	case HttpOutMsg::Type::HTTP_PUT:
		retValue = processPutTask(this, request,
			writeData,
			response->getData(),
			&responseCode,
			writeHeaderData,
			response->getInHeader(),
			responseMessage);
		break;

	case HttpOutMsg::Type::HTTP_DELETE:
		retValue = processDeleteTask(this, request,
			writeData,
			response->getData(),
			&responseCode,
			writeHeaderData,
			response->getInHeader(),
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
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Configure curl's timeout property
static bool configureCURL(HttpClient* client, CURL* handle, char* errorBuffer)
{
	if (!handle) {
		return false;
	}

	int32_t code;
	code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK) {
		return false;
	}
	code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, HttpClient::getSingleton().getTimeoutForRead());
	if (code != CURLE_OK) {
		return false;
	}
	code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, HttpClient::getSingleton().getTimeoutForConnect());
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
	bool init(HttpClient* client, HttpOutMsg* request, write_callback callback, void* stream, write_callback headerCallback, void* headerStream, char* errorBuffer)
	{
		if (!m_pCurl)
			return false;
		if (!configureCURL(client, m_pCurl, errorBuffer))
			return false;

		/* get custom header data (if set) */
		const vector<String>::type& headers = request->getOutHeaders();
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
static int processGetTask(HttpClient* client, HttpOutMsg* request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
	CURLRaii curl;
	bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
		&& curl.setOption(CURLOPT_FOLLOWLOCATION, true)
		&& curl.perform(responseCode);
	return ok ? 0 : 1;
}
//-----------------------------------------------------------------------
//Process POST Request
static int processPostTask(HttpClient* client, HttpOutMsg* request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
	CURLRaii curl;
	bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
		&& curl.setOption(CURLOPT_POST, 1)
		&& curl.setOption(CURLOPT_POSTFIELDS, request->getData())
		&& curl.setOption(CURLOPT_POSTFIELDSIZE, request->getDataSize())
		&& curl.perform(responseCode);
	return ok ? 0 : 1;
}
//-----------------------------------------------------------------------
//Process PUT Request
static int processPutTask(HttpClient* client, HttpOutMsg* request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
	CURLRaii curl;
	bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
		&& curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
		&& curl.setOption(CURLOPT_POSTFIELDS, request->getData())
		&& curl.setOption(CURLOPT_POSTFIELDSIZE, request->getDataSize())
		&& curl.perform(responseCode);
	return ok ? 0 : 1;
}
//-----------------------------------------------------------------------
//Process DELETE Request
static int processDeleteTask(HttpClient* client, HttpOutMsg* request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
	CURLRaii curl;
	bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
		&& curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
		&& curl.setOption(CURLOPT_FOLLOWLOCATION, true)
		&& curl.perform(responseCode);
	return ok ? 0 : 1;
}