//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __TabInfoFacade__
#define __TabInfoFacade__


#include "cocos2d.h"
#include "U2Prerequisites.h"
#include "U2Facade.h"
#include "TabInfoPrerequisites.h"


U2EG_NAMESPACE_USING


class TabInfoFacade : public Facade
{
public:
	/**
	* Constructor.
	*
	* <P>
	* This <code>Facade</code> implementation is a Multiton,
	* so you should not call the constructor
	* directly, but instead call the static Factory method,
	* passing the unique key for this instance
	* <code>Facade.getInstance( multitonKey )</code>
	*
	* @throws Error Error if instance for this Multiton key has already been constructed
	*
	*/
    explicit TabInfoFacade(const String& type, const String& name);

    /**
    * Virtual destructor.
    */
    virtual ~TabInfoFacade(void);

    static TabInfoFacade& getInstance(void);

    virtual void initializeController(void) override;
	
};


#endif /* defined(__TabInfoFacade__) */
