#ifndef __GameRoot_H__
#define __GameRoot_H__

#include "U2Core.h"
#include "U2Mvc.h"


class GameScene;


U2EG_NAMESPACE_USING


class GameRoot : public u2::Root
{
public:
    GameRoot();
    virtual ~GameRoot();

    virtual void enter() override;
    virtual void exit() override;
    virtual void pause() override;
    virtual void resume() override;

protected:
    virtual void _initialize() override;

protected:
    GameScene*      m_pGameScene;
};


#endif
