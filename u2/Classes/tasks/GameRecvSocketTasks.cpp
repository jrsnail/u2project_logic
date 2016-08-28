#include "GameRecvSocketTasks.h"

#include "GameWsClientImpl.h"
#include "ecs/GameComponents.h"
#include "ecs/GameSnapshot.h"
#include "GameSendSocketTasks.h"
#include "pb/SnapshotRsp.pb.h"


U2EG_NAMESPACE_USING


#define CHECK_RAPIDJSON_MEMBER(JsonValue, member)                       \
    if (!JsonValue.HasMember(member))                                   \
    {                                                                   \
        assert(0);                                                      \
        LogManager::getSingleton().stream(LML_CRITICAL)                 \
            << "[task error]: "                                         \
            << "Parse failed, as no member "                            \
            << member                                                   \
            << ". [ " << __FILE__                                       \
            << ", " << __FUNCTION__                                     \
            << ", " << __LINE__ << " ]";                                \
        break;                                                          \
    }

#define CHECK_RAPIDJSON_VALIDITY(result)                                \
    if (!result)                                                        \
    {                                                                   \
        assert(0);                                                      \
        LogManager::getSingleton().stream(LML_CRITICAL)                 \
            << "[task error]: "                                         \
            << "Parse failed, invalid json object. "                    \
            << "[ " << __FILE__                                         \
            << ", " << __FUNCTION__                                     \
            << ", " << __LINE__                                         \
            << " ]";                                                    \
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
    GameScene::getSingleton().end();
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
    GameScene::getSingleton().end();
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
        GameScene::getSingleton().start();
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SnapshotRST::SnapshotRST(const String& type, const String& name, const u2::String& guid)
    : RecvSocketTask(type, name, guid)
    , m_nCode(0)
{
}
//-----------------------------------------------------------------------
SnapshotRST::~SnapshotRST()
{
}
/*
//-----------------------------------------------------------------------
void SnapshotRST::deserialize()
{
    do 
    {
        
        std::string szJson(m_Data.begin(), m_Data.end());
        LogManager::getSingleton().stream(LML_TRIVIAL) << "SnapshotRST: " << szJson;

        u2uint64 ulStart = Root::getSingleton().getTimer()->getMilliseconds();
        rapidjson::Document document;
        document.Parse(szJson.c_str());

        CHECK_RAPIDJSON_VALIDITY(document.IsObject());

        CHECK_RAPIDJSON_MEMBER(document, "taskId");
        CHECK_RAPIDJSON_VALIDITY(document["taskId"].IsString());
        String szMsgId = document["taskId"].GetString();

        CHECK_RAPIDJSON_MEMBER(document, "version");
        CHECK_RAPIDJSON_VALIDITY(document["version"].IsString());
        m_szVersion = document["version"].GetString();

        CHECK_RAPIDJSON_MEMBER(document, "code");
        CHECK_RAPIDJSON_VALIDITY(document["code"].IsString());
        String szCode = document["code"].GetString();
        m_nCode = StringUtil::parseInt(szCode);

        if (m_nCode == 0)
        {
            u2uint64 ulStart1 = Root::getSingleton().getTimer()->getMicroseconds();
            CHECK_RAPIDJSON_MEMBER(document, "data");
            CHECK_RAPIDJSON_VALIDITY(document["data"].IsObject());
            const rapidjson::Value& dataJsonVal = document["data"];

            for (rapidjson::Value::ConstMemberIterator it = dataJsonVal.MemberBegin();
            it != dataJsonVal.MemberEnd(); it++)
            {
                String key = it->name.GetString();

                rapidjson::Document memberDoc;
                memberDoc.Parse(it->value.GetString());

                CHECK_RAPIDJSON_VALIDITY(memberDoc.IsObject());

                GameMovableSnapshot* pPlayerSnapshot = dynamic_cast<GameMovableSnapshot*>(
                    MovableSnapshotManager::getSingleton().reuseObject(GET_OBJECT_TYPE(GameMovableSnapshot)));

                if (_deserializeHero(memberDoc, pPlayerSnapshot))
                {
                    m_FrameSnapshot[pPlayerSnapshot->szGameObjGuid] = pPlayerSnapshot;
                }
                else
                {
                    MovableSnapshotManager::getSingleton().recycleObject(pPlayerSnapshot);
                    m_bDeserializeSucceed = false;
                }
            }
            u2uint64 ulDelta1 = Root::getSingleton().getTimer()->getMicroseconds() - ulStart1;
            LogManager::getSingleton().stream(LML_TRIVIAL) << "SnapshotRST::deserialize 1: " << ulDelta1;
        }
        else
        {
            assert(0);
            LogManager::getSingleton().stream(LML_CRITICAL)
                << "[task error]: code = "
                << m_nCode
                << ", json = "
                << szJson;
        }
        u2uint64 ulDelta = Root::getSingleton().getTimer()->getMilliseconds() - ulStart;
        LogManager::getSingleton().stream(LML_TRIVIAL) << "SnapshotRST::deserialize : " << ulDelta;

        m_bDeserializeSucceed = true;
    } while (0);

    m_bDeserializeSucceed = false;
}
*/

void SnapshotRST::deserialize()
{
    do
    {
        std::string szProtoBuf(m_Data.begin(), m_Data.end());
        LogManager::getSingleton().stream(LML_TRIVIAL) << "SnapshotRST: " << szProtoBuf;

        u2uint64 ulStart = Root::getSingleton().getTimer()->getMilliseconds();

        SnapshotRsp snapshotRsp;
        if (snapshotRsp.ParseFromString(szProtoBuf))
        {
            String szTaskId = snapshotRsp.taskid();
            m_szVersion = snapshotRsp.version();
            m_nCode = snapshotRsp.code();

            if (m_nCode == 0)
            {
                for (int i = 0; i < snapshotRsp.snapshots_size(); i++)
                {
                    GameMovableSnapshot* pPlayerSnapshot = dynamic_cast<GameMovableSnapshot*>(
                        MovableSnapshotManager::getSingleton().reuseObject(GET_OBJECT_TYPE(GameMovableSnapshot)));

                    const ::SnapshotRsp_MovableSnapshot& snapshot = snapshotRsp.snapshots(i);
                    pPlayerSnapshot->szPlayerId = StringUtil::toString(snapshot.userid());
                    pPlayerSnapshot->szGameObjGuid = StringUtil::toString(snapshot.heroid());
                    pPlayerSnapshot->szGameObjType = StringUtil::toString(snapshot.type());
                    pPlayerSnapshot->szPlayerName = snapshot.nickname();
                    pPlayerSnapshot->uCurHp = snapshot.hp();
                    pPlayerSnapshot->rCurSpeed = snapshot.speed();
                    pPlayerSnapshot->rAtkDistance = snapshot.effdistance();
                    pPlayerSnapshot->bAlive = snapshot.alive();
                    pPlayerSnapshot->ulTimestamp = snapshot.timestamp();
                    pPlayerSnapshot->v2Position.x = snapshot.x();
                    pPlayerSnapshot->v2Position.y = snapshot.y();
                    pPlayerSnapshot->v2Velocity.x = snapshot.vx();
                    pPlayerSnapshot->v2Velocity.y = snapshot.vy();

                    m_FrameSnapshot[pPlayerSnapshot->szGameObjGuid] = pPlayerSnapshot;
                }
            }
        }
        else
        {
            assert(0);
            LogManager::getSingleton().stream(LML_CRITICAL)
                << "[task error]: code = "
                << m_nCode
                << ", protobuf = "
                << szProtoBuf;
        }

        u2uint64 ulDelta = Root::getSingleton().getTimer()->getMilliseconds() - ulStart;
        LogManager::getSingleton().stream(LML_TRIVIAL) << "SnapshotRST::deserialize : " << ulDelta;

        m_bDeserializeSucceed = true;
    } while (0);

    m_bDeserializeSucceed = false;
}
//-----------------------------------------------------------------------
bool SnapshotRST::_deserializeHero(rapidjson::Value& jsonValue, GameMovableSnapshot* gameMovableSnapshot)
{
    do
    {
        CHECK_RAPIDJSON_MEMBER(jsonValue, "userId");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["userId"].IsUint64());
        u2uint64 ulPlayerId = jsonValue["userId"].GetUint64();
        gameMovableSnapshot->szPlayerId = StringUtil::toString(ulPlayerId);

        CHECK_RAPIDJSON_MEMBER(jsonValue, "heroId");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["heroId"].IsUint());
        u2uint32 ulGameObjId = jsonValue["heroId"].GetUint();
        gameMovableSnapshot->szGameObjGuid = StringUtil::toString(ulGameObjId);

        CHECK_RAPIDJSON_MEMBER(jsonValue, "type");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["type"].IsUint());
        u2uint32 uGameObjType = jsonValue["type"].GetUint();
        gameMovableSnapshot->szGameObjType = StringUtil::toString(uGameObjType);

        CHECK_RAPIDJSON_MEMBER(jsonValue, "nickName");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["nickName"].IsString());
        gameMovableSnapshot->szPlayerName = jsonValue["nickName"].GetString();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "hp");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["hp"].IsUint());
        gameMovableSnapshot->uCurHp = jsonValue["hp"].GetUint();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "speed");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["speed"].IsNumber());
        gameMovableSnapshot->rCurSpeed = jsonValue["speed"].GetDouble();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "effDistance");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["effDistance"].IsNumber());
        gameMovableSnapshot->rAtkDistance = jsonValue["effDistance"].GetDouble();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "alive");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["alive"].IsBool());
        gameMovableSnapshot->bAlive = jsonValue["alive"].GetBool();

        CHECK_RAPIDJSON_MEMBER(jsonValue, "point");
        CHECK_RAPIDJSON_VALIDITY(jsonValue["point"].IsObject());
        const rapidjson::Value& pointJsonVal = jsonValue["point"];

        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "timestamp");
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["timestamp"].IsUint64());
        gameMovableSnapshot->ulTimestamp = pointJsonVal["timestamp"].GetUint64();

        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "x");
        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "y");
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["x"].IsNumber());
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["y"].IsNumber());
        gameMovableSnapshot->v2Position.x = pointJsonVal["x"].GetDouble();
        gameMovableSnapshot->v2Position.y = pointJsonVal["y"].GetDouble();

        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "vx");
        CHECK_RAPIDJSON_MEMBER(pointJsonVal, "vy");
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["vx"].IsNumber());
        CHECK_RAPIDJSON_VALIDITY(pointJsonVal["vy"].IsNumber());
        gameMovableSnapshot->v2Velocity.x = pointJsonVal["vx"].GetDouble();
        gameMovableSnapshot->v2Velocity.y = pointJsonVal["vy"].GetDouble();

        return true;
    } while (0);

    return false;
}
//-----------------------------------------------------------------------
void SnapshotRST::run()
{
    deserialize();
    if (m_nCode == 0)
    {
        u2uint64 ulStart = Root::getSingleton().getTimer()->getMilliseconds();
        Scene::getSingleton().addFrameSnapshot(&m_FrameSnapshot);
        u2uint64 ulDelta = Root::getSingleton().getTimer()->getMilliseconds() - ulStart;
        LogManager::getSingleton().stream(LML_TRIVIAL) << "SnapshotRST::addFrameSnapshot : " << ulDelta;
    }
    else
    {
        LogManager::getSingleton().stream(LML_NORMAL)
            << "Play failed, code = " << m_nCode
            << ", " << m_szMsg
            << "[ " << __FUNCTION__ << ", " << __LINE__ << " ]";
    }
}