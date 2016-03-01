//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __ApplicationFacade__
#define __ApplicationFacade__


#include "U2GameBase.h"


U2EG_NAMESPACE_USING


class ApplicationFacade : public Facade, public Singleton < ApplicationFacade >
{
public:
    ApplicationFacade(const u2::String& type, const u2::String& name);

    virtual ~ApplicationFacade(void);

    virtual void initializeFacade(void) override;

    virtual void initializeController(void) override;

	virtual void initializeModel(void) override;

	virtual void initializeView(void) override;

    void initializeContextQueue(void);
	
    void startup();

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
	static ApplicationFacade& getSingleton(void);

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
	static ApplicationFacade* getSingletonPtr(void);
};


#endif /* defined(__ApplicationFacade__) */
