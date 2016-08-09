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


U2EG_NAMESPACE_BEGIN


class PredefinedFacade : public Facade
{
public:
    PredefinedFacade(const String& type, const String& name, const String& guid = BLANK);

    virtual ~PredefinedFacade(void);

    virtual void initializeController(void) override;

	virtual void initializeModel(void) override;

	virtual void initializeView(void) override;
	
private:
	static PredefinedFacade*			ms_pSingleton;
};


U2EG_NAMESPACE_END


#endif /* defined(__U2PredefinedFacade__) */
