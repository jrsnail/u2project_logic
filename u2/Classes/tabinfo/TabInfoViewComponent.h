//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __TabInfoViewComponent__
#define __TabInfoViewComponent__


#include "cocos2d.h"
#include "U2Prerequisites.h"
#include "U2CocosViewComponent.h"
#include "TabInfoPrerequisites.h"


U2EG_NAMESPACE_USING


class TabInfoViewComponent : public CocosViewComponent
{
public:
    TabInfoViewComponent(const String& type, const String& name);

    virtual ~TabInfoViewComponent(void);

    virtual u2::String getUiName() const override;

};


#endif /* defined(__TabInfoViewComponent__) */
