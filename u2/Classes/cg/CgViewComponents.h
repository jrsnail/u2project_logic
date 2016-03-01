//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __CgViewComponents__
#define __CgViewComponents__


#include "U2GameBase.h"
#include "CgPrerequisites.h"


U2EG_NAMESPACE_USING


class CgViewComponent : public CocosViewComponent
{
public:
	CgViewComponent(const u2::String& type, const u2::String& name);

	virtual ~CgViewComponent(void);

    virtual const u2::String& getUiName() const override;

    void runCgAction();

public:
    static const u2::String AN_CgAction;
};


class StartPageViewComponent : public CocosViewComponent
{
public:
    StartPageViewComponent(const u2::String& type, const u2::String& name);

    virtual ~StartPageViewComponent(void);

    virtual const u2::String& getUiName() const override;

    void runStartAction();

public:
    static const u2::String AN_StartAction;
};


#endif /* defined(__CgViewComponents__) */
