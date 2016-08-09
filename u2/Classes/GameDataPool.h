#ifndef __GameDataPool__
#define __GameDataPool__


#include "U2Core.h"


U2EG_NAMESPACE_USING


class GameMovableSnapshot : public MovableSnapshot
{
public:
    GameMovableSnapshot()
        : MovableSnapshot()
        , nCurHp(0)
        , uCurSpeed(0)
        , uAtkDistance(0)
    {

    }

    u2::String      szPlayerId;
    u2::String      szPlayerName;
    u2int32         nCurHp;
    u2uint32        uCurSpeed;
    u2uint32        uAtkDistance;
};



#endif /* defined(__GameDataPool__) */
