#include "GameRecvSocketTasks.h"

#include "GameWsClientImpl.h"
#include "ecs/GameComponents.h"


U2EG_NAMESPACE_USING


#define CHECK_JSON_MEMBER(JsonValue, member)                            \
    if (!JsonValue.isMember(member))                                    \
    {                                                                   \
        LogManager::getSingleton().stream(LML_CRITICAL)                 \
            << "[task error]: "                                         \
            << "Parse failed, as no member "                            \
            << member                                                   \
            << ". [ " << __FUNCTION__ << ", " << __LINE__ << " ]";      \
        break;                                                          \
    }


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameWsCloseRST::GameWsCloseRST(const u2::String& type, const u2::String& name, const u2::String& guid)
    : WsCloseRST(type, name, guid)
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
GameWsErrorRST::GameWsErrorRST(const u2::String& type, const u2::String& name, const u2::String& guid)
    : WsErrorRST(type, name, guid)
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
GameWsOpenRST::GameWsOpenRST(const u2::String& type, const u2::String& name, const u2::String& guid)
    : WsOpenRST(type, name, guid)
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
        //pWsTaskLoop->startHeartBeat();
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SnapshotRST::SnapshotRST(const String& type, const String& name, const u2::String& guid)
    : RecvSocketTask(type, name, guid)
    , m_nCode(0)
    , m_ulTimestamp(0L)
{
}
//-----------------------------------------------------------------------
SnapshotRST::~SnapshotRST()
{
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

        m_pSceneSnapshot = U2_NEW SceneSnapshot;

        CHECK_JSON_MEMBER(dataJsonVal, "timestamp");
        m_ulTimestamp = dataJsonVal["timestamp"].asUInt64();

        CHECK_JSON_MEMBER(dataJsonVal, "heros");
        Json::Value herosJsonVal = dataJsonVal["heros"];

        for (Json::ArrayIndex i = 0; i < herosJsonVal.size(); i++)
        {
            GameMovableSnapshot* pPlayerSnapshot = U2_NEW GameMovableSnapshot;
            if (_deserializeHero(herosJsonVal[i], pPlayerSnapshot))
            {
                m_pSceneSnapshot->addMovableSnapshot(pPlayerSnapshot);
            }
            else
            {
                U2_DELETE(pPlayerSnapshot);
                m_bDeserializeSucceed = false;
            }
        }

        m_bDeserializeSucceed = true;
    } while (0);

    m_bDeserializeSucceed = false;
}
//-----------------------------------------------------------------------
bool SnapshotRST::_deserializeHero(Json::Value& jsonValue, GameMovableSnapshot* gameMovableSnapshot)
{
    do
    {
        CHECK_JSON_MEMBER(jsonValue, "userId");
        u2uint64 ulPlayerId = jsonValue["userId"].asUInt64();
        gameMovableSnapshot->szPlayerId = StringUtil::toString(ulPlayerId);

        CHECK_JSON_MEMBER(jsonValue, "heroId");
        u2uint32 ulGameObjId = jsonValue["heroId"].asUInt();
        gameMovableSnapshot->szGameObjGuid = StringUtil::toString(ulGameObjId);

        CHECK_JSON_MEMBER(jsonValue, "nickName");
        gameMovableSnapshot->szPlayerName = jsonValue["nickName"].asString();

        CHECK_JSON_MEMBER(jsonValue, "hp");
        gameMovableSnapshot->uCurHp = jsonValue["hp"].asUInt();

        CHECK_JSON_MEMBER(jsonValue, "speed");
        gameMovableSnapshot->uCurSpeed = jsonValue["speed"].asUInt();

        CHECK_JSON_MEMBER(jsonValue, "effDistance");
        gameMovableSnapshot->uAtkDistance = jsonValue["effDistance"].asUInt();

        CHECK_JSON_MEMBER(jsonValue, "alive");
        gameMovableSnapshot->bAlive = jsonValue["alive"].asBool();

        CHECK_JSON_MEMBER(jsonValue, "point");
        Json::Value pointJsonVal = jsonValue["point"];

        CHECK_JSON_MEMBER(pointJsonVal, "timestamp");
        gameMovableSnapshot->ulTimestamp = pointJsonVal["timestamp"].asUInt64();

        CHECK_JSON_MEMBER(pointJsonVal, "x");
        CHECK_JSON_MEMBER(pointJsonVal, "y");
        gameMovableSnapshot->v2Position.x = pointJsonVal["x"].asFloat();
        gameMovableSnapshot->v2Position.y = pointJsonVal["y"].asFloat();

        CHECK_JSON_MEMBER(pointJsonVal, "vx");
        CHECK_JSON_MEMBER(pointJsonVal, "vy");
        gameMovableSnapshot->v2Velocity.x = pointJsonVal["vx"].asFloat();
        gameMovableSnapshot->v2Velocity.y = pointJsonVal["vy"].asFloat();

        return true;
    } while (0);

    return false;
}
//-----------------------------------------------------------------------
void SnapshotRST::run()
{
    /*
    deserialize();
    if (m_nCode == 0)
    {
        // create every game object
        SceneSnapshot::MovableSnapshotListIterator it = m_pSceneSnapshot->retrieveAllMovableSnapshots();
        while (it.hasMoreElements())
        {
            GameMovableSnapshot* pMovableSs = static_cast<GameMovableSnapshot*>(it.getNext());
            if (pMovableSs == nullptr)
            {
                assert(0);
            }
            else
            {
                // the first cal
                pMovableSs->uCalCounter = 0;

                // create game object
                GameObject* pGameObj = GameObjectManager::getSingleton().createObject(
                    pMovableSs->szGameObjType, pMovableSs->szPlayerId, pMovableSs->szGameObjGuid);

                // position component
                PositionComponent* pPositionComp = dynamic_cast<PositionComponent*>(
                    pGameObj->retrieveComponentByType("component_position"));
                if (pPositionComp == nullptr)
                {
                    assert(0);
                }
                else
                {
                    pPositionComp->v2Pos = pMovableSs->v2Position;
                }

                // velocity component
                VelocityComponent* pVelocityComp = dynamic_cast<VelocityComponent*>(
                    pGameObj->retrieveComponentByType("component_velocity"));
                if (pVelocityComp == nullptr)
                {
                    assert(0);
                }
                else
                {
                    pVelocityComp->v2Velocity = pMovableSs->v2Velocity;
                }

                // speed component
                SpeedComponent* pSpeedComp = dynamic_cast<SpeedComponent*>(
                    pGameObj->retrieveComponentByType("component_speed"));
                if (pSpeedComp == nullptr)
                {
                    assert(0);
                }
                else
                {
                    pSpeedComp->fSpeed = pMovableSs->v2Velocity.getLength();
                }

                // speed direction component
                SpeedDirComponent* pSpeedDirComp = dynamic_cast<SpeedDirComponent*>(
                    pGameObj->retrieveComponentByType("component_speed_dir"));
                if (pSpeedDirComp == nullptr)
                {
                    assert(0);
                }
                else
                {
                    pSpeedDirComp->v2Dir = pMovableSs->v2Velocity;
                    pSpeedDirComp->v2Dir.normalize();
                }

                // hp component
                HpComponent* pHpComp = dynamic_cast<HpComponent*>(
                    pGameObj->retrieveComponentByType("component_hp"));
                if (pHpComp == nullptr)
                {
                    assert(0);
                }
                else
                {
                    pHpComp->uHp = pMovableSs->uCurHp;
                }

                // joystick component
                String szSelfPlayerGuid;
                bool bSuc = DATAPOOL(ON_DataPool_Memory)->loadMemoryStringData("SelfPlayerGuid", szSelfPlayerGuid);
                if (szSelfPlayerGuid == pMovableSs->szPlayerId)
                {
                    pGameObj->addComponent(ComponentManager::getSingleton().createObject("component_joystick"));
                }
            }
        }
    }
    else
    {
        LogManager::getSingleton().stream(LML_NORMAL)
            << "Play failed, code = " << m_nCode
            << ", " << m_szMsg
            << "[ " << __FUNCTION__ << ", " << __LINE__ << " ]";
    }
    */
}