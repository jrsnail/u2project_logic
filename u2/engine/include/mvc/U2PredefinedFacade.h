//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2PredefinedFacade__
#define __U2PredefinedFacade__


#include "U2Core.h"
#include "U2Facade.h"


U2EG_NAMESPACE_USING


class PredefinedFacade : public Facade, public Singleton < PredefinedFacade >
{
public:
    PredefinedFacade(const String& type, const String& name);

    virtual ~PredefinedFacade(void);

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
    static PredefinedFacade& getSingleton(void);

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
    static PredefinedFacade* getSingletonPtr(void);
};


#endif /* defined(__U2PredefinedFacade__) */
