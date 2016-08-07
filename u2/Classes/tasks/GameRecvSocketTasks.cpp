#include "GameRecvSocketTasks.h"

#include "GameWsClientImpl.h"


U2EG_NAMESPACE_USING


#define CHECK_JSON_MEMBER(JsonValue, member)                            \
    if (!JsonValue.isMember(member))                                    \
    {                                                                   \
        LogManager::getSingleton().stream(LML_CRITICAL)                 \
            << "[task error]: SnapshotRST parse failed, as no member"   \
            << member;                                                  \
        break;                                                          \
    }


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
SnapshotRST::SnapshotRST(const String& type, const String& name)
    : RecvSocketTask(type, name)
    , m_nCode(0)
    , m_ulTimestamp(0L)
{
}
//-----------------------------------------------------------------------
SnapshotRST::~SnapshotRST()
{
    for (vector<PlayerSnapshot*>::iterator it = m_PlayerSnapshots.begin(); 
    it != m_PlayerSnapshots.end(); it++)
    {
        U2_FREE(*it, MEMCATEGORY_GENERAL);
    }
    m_PlayerSnapshots.clear();
}
//-----------------------------------------------------------------------
void SnapshotRST::deserialize()
{
    do 
    {
        std::string szJson(m_Data.begin(), m_Data.end());

        Json::Reader reader;
        Json::Value rootJsonVal;
        if (!reader.parse(szJson, rootJsonVal))
        {
            LogManager::getSingleton().stream(LML_CRITICAL)
                << "[task error]: SnapshotRST parse failed!";
            break;
        }

        CHECK_JSON_MEMBER(rootJsonVal, "TaskId");
        std::string szMsgId = rootJsonVal["TaskId"].asString();

        CHECK_JSON_MEMBER(rootJsonVal, "code");
        m_nCode = rootJsonVal["code"].asInt();

        CHECK_JSON_MEMBER(rootJsonVal, "message");
        m_szMsg = rootJsonVal["message"].asString();

        CHECK_JSON_MEMBER(rootJsonVal, "data");
        Json::Value dataJsonVal = rootJsonVal["data"];

        CHECK_JSON_MEMBER(dataJsonVal, "timestamp");
        m_ulTimestamp = dataJsonVal["timestamp"].asUInt64();

        CHECK_JSON_MEMBER(dataJsonVal, "heros");
        Json::Value herosJsonVal = dataJsonVal["heros"];

        for (Json::ArrayIndex i = 0; i < herosJsonVal.size(); i++)
        {
            PlayerSnapshot* pPlayerSnapshot = U2_ALLOC_T(PlayerSnapshot, sizeof(PlayerSnapshot), MEMCATEGORY_GENERAL);
            if (_deserializeHero(herosJsonVal[i], *pPlayerSnapshot))
            {
                m_PlayerSnapshots.push_back(pPlayerSnapshot);
            }
            else
            {
                U2_FREE(pPlayerSnapshot, MEMCATEGORY_GENERAL);
                m_bDeserializeSucceed = false;
            }
        }

        m_bDeserializeSucceed = true;
    } while (0);

    m_bDeserializeSucceed = false;
}
//-----------------------------------------------------------------------
bool SnapshotRST::_deserializeHero(Json::Value& jsonValue, PlayerSnapshot& playerSnapshot)
{
    do 
    {
        CHECK_JSON_MEMBER(jsonValue, "userId");
        u2uint64 ulPlayerId = jsonValue["userId"].asUInt64();
        playerSnapshot.szPlayerId = StringUtil::toString(ulPlayerId);

        CHECK_JSON_MEMBER(jsonValue, "heroId");
        u2uint32 ulGameObjId = jsonValue["heroId"].asUInt();
        playerSnapshot.szGameObjGuid = StringUtil::toString(ulGameObjId);

        CHECK_JSON_MEMBER(jsonValue, "nickName");
        playerSnapshot.szPlayerName = jsonValue["nickName"].asString();

        CHECK_JSON_MEMBER(jsonValue, "hp");
        playerSnapshot.nCurHp = jsonValue["hp"].asInt();

        CHECK_JSON_MEMBER(jsonValue, "speed");
        playerSnapshot.uCurSpeed = jsonValue["speed"].asUInt();

        CHECK_JSON_MEMBER(jsonValue, "effDistance");
        playerSnapshot.uAtkDistance = jsonValue["effDistance"].asUInt();

        CHECK_JSON_MEMBER(jsonValue, "alive");
        playerSnapshot.bAlive = jsonValue["alive"].asBool();

        CHECK_JSON_MEMBER(jsonValue, "point");
        Json::Value pointJsonVal = jsonValue["point"];

        CHECK_JSON_MEMBER(pointJsonVal, "timestamp");
        playerSnapshot.ulTimestamp = pointJsonVal["timestamp"].asUInt64();

        CHECK_JSON_MEMBER(pointJsonVal, "x");
        CHECK_JSON_MEMBER(pointJsonVal, "y");
        playerSnapshot.v2Position.x = pointJsonVal["x"].asFloat();
        playerSnapshot.v2Position.y = pointJsonVal["y"].asFloat();

        CHECK_JSON_MEMBER(pointJsonVal, "vx");
        CHECK_JSON_MEMBER(pointJsonVal, "vy");
        playerSnapshot.v2Velocity.x = pointJsonVal["vx"].asFloat();
        playerSnapshot.v2Velocity.y = pointJsonVal["vy"].asFloat();

        return true;
    } while (0);

    return false;
}
//-----------------------------------------------------------------------
void SnapshotRST::run()
{

}