#ifndef __GameDataPool__
#define __GameDataPool__


#include "U2Core.h"


U2EG_NAMESPACE_USING


class GameMovableSnapshot : public MovableSnapshot
{
public:
    GameMovableSnapshot(const String& type, const String& name = BLANK, const String& guid = BLANK)
        : MovableSnapshot(type, name, guid)
        , uCurHp(0)
        , uCurSpeed(0)
        , uAtkDistance(0)
    {

    }

    u2::String      szPlayerId;
    u2::String      szPlayerName;
    u2uint32        uCurHp;
    u2uint32        uCurSpeed;
    u2uint32        uAtkDistance;
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



#endif /* defined(__GameDataPool__) */
