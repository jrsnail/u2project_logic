//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __AppFacade__
#define __AppFacade__


#include "U2GameBase.h"


U2EG_NAMESPACE_USING


class AppFacade : public Facade
{
public:
    AppFacade(const std::string& type, const std::string& name);

    virtual ~AppFacade(void);

    virtual void initializeFacade(void) override;

    virtual void initializeController(void) override;

	virtual void initializeModel(void) override;

	virtual void initializeView(void) override;

    virtual void initializeHerald(void) override;

    void initializeContextQueue(void);
	
    void startup();

private:
	static AppFacade*		ms_pSingleton;
};


#endif /* defined(__ApplicationFacade__) */
