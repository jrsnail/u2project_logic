#ifndef __GameSnapshot__
#define __GameSnapshot__


#include "U2Core.h"


U2EG_NAMESPACE_USING


class GameMovableSnapshot : public MovableSnapshot
{
public:
    GameMovableSnapshot(const String& type, const String& name = BLANK, const String& guid = BLANK)
        : MovableSnapshot(type, name, guid)
        , uCurHp(0)
        , rCurSpeed(0)
        , rAtkDistance(0)
    {

    }

    u2::String      szPlayerId;
    u2::String      szPlayerName;
    u2uint32        uCurHp;
    u2real          rCurSpeed;
    u2real          rAtkDistance;
};


class GameControlSnapshot : public ControlSnapshot
{
public:
    GameControlSnapshot(const String& type, const String& name = BLANK, const String& guid = BLANK)
        : ControlSnapshot(type, name, guid)
    {

    }

    u2::String      szRoomGuid;
    u2::String      szAttackedPlayerGuid;
};


class GameScene : public Scene
{
public:
    GameScene();

    virtual ~GameScene();

    virtual void start();

    virtual void end();

    void connect();

    void disconnect();

protected:
    virtual void _updateGameObjWithSnapshot() override;
    virtual void _onUpdate(float dt) override;
    virtual void _initGameObj(GameObject* gameObj, MovableSnapshot* movableSnapshot) override;

protected:
    WsTaskLoop* m_pWsTaskLoop;
};


#endif /* defined(__GameSnapshot__) */
