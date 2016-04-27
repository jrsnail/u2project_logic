#ifndef __U2NetMsg__
#define __U2NetMsg__


#include "U2Prerequisites.h"
#include "U2Object.h"
#include "U2PoolingObjectManager.h"
#include "U2TypedObjectManager.h"


U2EG_NAMESPACE_BEGIN


class Msg : public ReusableObject
{
public:
	Msg(const String& type)
		: ReusableObject(type)
	{}
	virtual ~Msg() {};

	void initialize(const String& protocolName)
	{
		m_szProtocolName = protocolName;
	}

	virtual String serialize() = 0;
	virtual void deserialize(const String& str) = 0;

// 	virtual void preRecycleByPool() override 
// 	{
// 		ReusableObject::preRecycleByPool();
// 	};
// 	virtual void postReuseFromPool() override 
// 	{
// 		ReusableObject::postReuseFromPool();
// 	};

	const String& getProtocolName() const { return m_szProtocolName; }

protected:
	String		m_szProtocolName;
};


class Protocol : public ReusableObject
{
public:
	Protocol(const String& type);
	virtual ~Protocol();

	void initialize(const String& outType, const String& inType);
	inline bool isPaired() { return _isPaired() || isPredicable(); };
	virtual bool isPredicable() = 0;

	virtual String serialize();
	virtual void deserialize(const String& str);

	virtual void preRecycleByPool() override;
	virtual void postReuseFromPool() override;

	inline void setOutMsg(Msg* msg);
	inline void setInMsg(Msg* msg);
	inline void setPredictedMsg(Msg* msg);

	inline Msg& getOutMsg() const;
	inline Msg& getInMsg() const;
	inline Msg& getPredictedMsg() const;

protected:
	virtual bool _isPaired() = 0;

protected:
	Msg*		m_pOutMsg;
	Msg*		m_pInMsg;
	Msg*		m_pPredictedMsg;
	
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



class ProtocolManager : public PoolingObjectManager<Protocol>, public Singleton < ProtocolManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	ProtocolManager();

public:
    /** Default destructor.
    */
    virtual ~ProtocolManager();

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
    static ProtocolManager& getSingleton(void);

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
    static ProtocolManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Command__) */
