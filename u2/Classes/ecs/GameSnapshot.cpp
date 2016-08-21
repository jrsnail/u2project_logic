#include "GameSnapshot.h"

#include "tasks/GameSendSocketTasks.h"
#include "tasks/GameWsClientImpl.h"
#include "ecs/GameComponents.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GameScene::GameScene()
    : Scene()
    , m_pWsTaskLoop(nullptr)
{
}
//-----------------------------------------------------------------------
GameScene::~GameScene()
{
    end();
}
//-----------------------------------------------------------------------
void GameScene::connect()
{
    // create websocket connection
    if (m_pWsTaskLoop == nullptr)
    {
        m_pWsTaskLoop = dynamic_cast<WsTaskLoop*>(
            TaskLoopManager::getSingleton().createObject(GET_OBJECT_TYPE(GameWsTaskLoop), "websocket")
            );
        //m_pWsTaskLoop->setUrl("ws://echo.websocket.org");
        m_pWsTaskLoop->setUrl("ws://10.60.81.51:9008");
        m_pWsTaskLoop->run();
    }
}
//-----------------------------------------------------------------------
void GameScene::disconnect()
{
    if (m_pWsTaskLoop != nullptr)
    {
        m_pWsTaskLoop->quit();
        TaskLoopManager::getSingleton().destoryObject(m_pWsTaskLoop);
        m_pWsTaskLoop = nullptr;
    }
}
//-----------------------------------------------------------------------
void GameScene::start()
{
    // frame listener
    u2::FrameListenerCollection::getSingleton().addFrameListener(this
        , std::bind(&GameScene::_onUpdate, this, std::placeholders::_1));
}
//-----------------------------------------------------------------------
void GameScene::end()
{
    u2::FrameListenerCollection::getSingleton().removeFrameListener(this);
}
//-----------------------------------------------------------------------
void GameScene::_onUpdate(float dt)
{
    _updateGameObjWithSnapshot();

    // send synchronize task every frame
    TaskLoop* pWsTaskLoop = TaskLoopManager::getSingleton().retrieveObjectByName("websocket");
    if (pWsTaskLoop == nullptr)
    {
        assert(0);
    }
    else
    {
        Task* pTask = TaskManager::getSingleton().createObject(GET_OBJECT_TYPE(MoveSST));
        pWsTaskLoop->postTask(pTask);
    }
}
//-----------------------------------------------------------------------
void GameScene::_updateGameObjWithSnapshot()
{
    Scene::_updateGameObjWithSnapshot();
}
//-----------------------------------------------------------------------
void GameScene::_initGameObj(GameObject* gameObj, MovableSnapshot* movableSnapshot)
{
    GameMovableSnapshot* pMovableSs = dynamic_cast<GameMovableSnapshot*>(movableSnapshot);
    if (pMovableSs == nullptr)
    {
        assert(0);
    }

    // the first cal
    pMovableSs->uCalCounter = 1;

    // position component
    PositionComponent* pPositionComp = dynamic_cast<PositionComponent*>(
        gameObj->retrieveComponentByType("component_position"));
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
        gameObj->retrieveComponentByType("component_velocity"));
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
        gameObj->retrieveComponentByType("component_speed"));
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
        gameObj->retrieveComponentByType("component_speed_dir"));
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
        gameObj->retrieveComponentByType("component_hp"));
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
        gameObj->addComponent(ComponentManager::getSingleton().createObject("component_predict_self"));
    }
    else
    {
        gameObj->addComponent(ComponentManager::getSingleton().createObject("component_predict_other"));
    }
}