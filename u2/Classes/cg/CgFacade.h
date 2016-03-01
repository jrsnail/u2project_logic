//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __CgFacade__
#define __CgFacade__


#include "U2GameBase.h"
#include "CgPrerequisites.h"


U2EG_NAMESPACE_USING


class CgFacade : public Facade, public Singleton < CgFacade >
{
public:
    CgFacade(const u2::String& type, const u2::String& name);

    virtual ~CgFacade(void);

    virtual void initializeController(void) override;

	virtual void initializeModel(void) override;

	virtual void initializeView(void) override;
	
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
    static CgFacade& getSingleton(void);

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
	static CgFacade* getSingletonPtr(void);
};


#endif /* defined(__CgFacade__) */
