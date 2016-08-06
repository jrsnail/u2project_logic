#include "RecvSocketTasks.h"

#include "GameWsClientImpl.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameWsCloseRST::GameWsCloseRST(const u2::String& type, const u2::String& name)
    : WsCloseRST(type, name)
{

}
//-----------------------------------------------------------------------
GameWsCloseRST::~GameWsCloseRST()
{

}
//-----------------------------------------------------------------------
void GameWsCloseRST::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameWsErrorRST::GameWsErrorRST(const u2::String& type, const u2::String& name)
    : WsErrorRST(type, name)
{

}
//-----------------------------------------------------------------------
GameWsErrorRST::~GameWsErrorRST()
{

}
//-----------------------------------------------------------------------
void GameWsErrorRST::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameWsOpenRST::GameWsOpenRST(const u2::String& type, const u2::String& name)
    : WsOpenRST(type, name)
{

}
//-----------------------------------------------------------------------
GameWsOpenRST::~GameWsOpenRST()
{

}
//-----------------------------------------------------------------------
void GameWsOpenRST::run()
{
    GameWsTaskLoop* pWsTaskLoop = dynamic_cast<GameWsTaskLoop*>(
        TaskLoopManager::getSingleton().retrieveObjectByName("websocket")
        );
    if (pWsTaskLoop == nullptr)
    {
        assert(0);
    }
    else
    {
        pWsTaskLoop->startHeartBeat();
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameWsHeartBeatSST::GameWsHeartBeatSST(const u2::String& type, const u2::String& name)
    : WsHeartBeatSST(type, name)
{

}
//-----------------------------------------------------------------------
GameWsHeartBeatSST::~GameWsHeartBeatSST()
{

}
//-----------------------------------------------------------------------
void GameWsHeartBeatSST::run()
{

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SnapshotRST::SnapshotRST(const String& type, const String& name)
    : RecvSocketTask(type, name)
{
}
//-----------------------------------------------------------------------
SnapshotRST::~SnapshotRST()
{
}