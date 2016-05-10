#ifndef __U2NetMsg__
#define __U2NetMsg__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2PoolingObjectManager.h"
#include "U2TypedObjectManager.h"


U2EG_NAMESPACE_BEGIN


class Msg : public ReusableObject
{
public:
	Msg(const String& type);
	virtual ~Msg();

	virtual void initialize(const String& outMsgName) = 0;

	const String& getOutMsgName() const { return m_szOutMsgName; }

	virtual String serialize() = 0;
	virtual void deserialize(const String& str) = 0;

	virtual void preRecycleByPool() override 
	{
		m_szOutMsgName = BLANK;
		ReusableObject::preRecycleByPool();
	};
	virtual void postReuseFromPool() override 
	{
		ReusableObject::postReuseFromPool();
		m_szOutMsgName = BLANK;
	};

protected:
	String		m_szOutMsgName;
};


class OutMsg : public Msg
{
public:
	enum MsgType
	{
		MT_SYNCHRONOUS,
		MT_ASYNCHRONOUS,
		MT_PREDICTABLE
	};

public:
	OutMsg(const String& type);
	virtual ~OutMsg();

	virtual void initialize(const String& outMsgName) override;

	/** Get the request data pointer of HttpRequest object.
	@return char* the request data pointer.
	*/
	inline const u2char* const getData();

	/** Get the size of request data
	@return ssize_t the size of request data
	*/
	inline size_t getDataSize();

	inline OutMsg::MsgType getMsgType() const;

	inline bool isPaired() { return _isPaired() || isPredicable(); };
	virtual bool isPredicable() = 0;
	virtual bool canConcate(const OutMsg* msg) = 0;

	inline void setInMsg(InMsg* msg);
	inline void setPredictedMsg(InMsg* msg);

	inline InMsg* getInMsg() const;
	inline InMsg* getPredictedMsg() const;

	virtual void preRecycleByPool() override;
	virtual void postReuseFromPool() override;

	virtual void predict() = 0;
	virtual void execute() = 0;

protected:
	virtual bool _isPaired() = 0;

protected:
	vector<u2char>::type	m_Data;
	MsgType					m_eMsgType;
	InMsg*					m_pInMsg;
	InMsg*					m_pPredictedMsg;
};


class InMsg : public Msg
{
public:
	InMsg(const String& type);
	virtual ~InMsg();

	/** Get the request data pointer of HttpRequest object.
	@return char* the request data pointer.
	*/
	inline u2char* getData();

	/** Get the size of request data
	@return ssize_t the size of request data
	*/
	inline size_t getDataSize();

	virtual void preRecycleByPool() override;
	virtual void postReuseFromPool() override;

protected:
	vector<u2char>::type	m_Data;
};


class MsgManager : public PoolingObjectManager<Msg>, public Singleton < MsgManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	MsgManager();

public:
	/** Default destructor.
	*/
	virtual ~MsgManager();

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
	static MsgManager& getSingleton(void);

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
	static MsgManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Command__) */
