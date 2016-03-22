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


class ApplicationFacade : public Facade
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

private:
	static ApplicationFacade*		ms_pSingleton;
};


#endif /* defined(__ApplicationFacade__) */
