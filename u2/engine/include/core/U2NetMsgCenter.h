#ifndef __U2NetMsgCenter_H__
#define __U2NetMsgCenter_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Singleton.h"
#include "U2SimpleObjectManager.h"


U2EG_NAMESPACE_BEGIN


class Msg;
class OutMsg;


class MsgCenter : public Object
{
public:
    /** Default constructor.
    */
	MsgCenter(const String& type, const String& name)
		: Object(type, name)
	{

	}

    /** Default destructor.
    */
	virtual ~MsgCenter()
	{

	}

	virtual void pushOutMsg(OutMsg* msg) = 0;
	virtual void pushInMsg(Msg* msg) = 0;
	virtual void onUpdate(float dt) = 0;

};


class MsgCenterManager : public Singleton < MsgCenterManager >, public SimpleObjectManager<MsgCenter>
{
public:
	MsgCenterManager();
	virtual ~MsgCenterManager();

	void pushOutMsg(const String& centerName, OutMsg* msg);
	void pushInMsg(const String& centerName, Msg* msg);

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
	static MsgCenterManager& getSingleton(void);

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
	static MsgCenterManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END


#endif // __U2TypedObjectManager_H__
