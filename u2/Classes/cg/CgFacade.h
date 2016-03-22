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


class CgFacade : public Facade
{
public:
    CgFacade(const u2::String& type, const u2::String& name);

    virtual ~CgFacade(void);

    virtual void initializeController(void) override;

	virtual void initializeModel(void) override;

	virtual void initializeView(void) override;
	
private:
	static CgFacade*			ms_pSingleton;
};


#endif /* defined(__CgFacade__) */
