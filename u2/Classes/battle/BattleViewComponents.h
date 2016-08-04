#ifndef __BattleViewComponents_H__
#define __BattleViewComponents_H__


#include "U2Core.h"
#include "U2Mvc.h"
#include "cocos2d.h"
#include "U2CocosViewComponent.h"



class BattleViewComponent : public CocosViewComponent
{
public:
    BattleViewComponent(const u2::String& type, const u2::String& name);

    virtual ~BattleViewComponent(void);

};


class RankListViewComponent : public CocosViewComponent
{
public:
    RankListViewComponent(const u2::String& type, const u2::String& name);

    virtual ~RankListViewComponent(void);

};


#endif /* defined(__BattleViewComponents_H__) */
