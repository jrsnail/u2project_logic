#include "U2Root.h"

#include "U2LogManager.h"
#include "U2Component.h"
#include "U2GameObject.h"
#include "U2Timer.h"
#include "U2Snapshot.h"



U2EG_NAMESPACE_USING


    
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> Root* Singleton<Root>::msSingleton = 0;
Root* Root::getSingletonPtr(void)
{
    assert(msSingleton != nullptr);
    return msSingleton;
}
//-----------------------------------------------------------------------
Root& Root::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
Root::Root()
{
    /*
    initGameFactories();

    // Create log manager and default log file if there is no log manager yet
    if (u2::LogManager::getSingletonPtr() == nullptr)
    {
        m_pLogManager = U2_NEW u2::LogManager();
        m_pLogManager->createLog("u2.log", true, true);
#if U2_DEBUG_MODE == 1
        m_pLogManager->setLogDetail(LoggingLevel::LL_BOREME);
#endif
    }

#if U2_PLATFORM == U2_PLATFORM_ANDROID
    AndroidLogListener* mAndroidLogger = U2_NEW AndroidLogListener();
    m_pLogManager->getDefaultLog()->addListener(mAndroidLogger);
#endif

    // data pool
    DataPoolManager::getSingleton().createObject(GET_OBJECT_TYPE(DataPool), ON_DataPool_Memory);

    
    // script manager
    u2::ScriptManager* pScriptManager = new u2::LuaScriptManager;
    // component manager
    m_pComponentManager = new GameComponentManager;
    // game object manager
    m_pGameObjectManager = new u2::GameObjectManager;


    // Logic task loop
    LogicTaskLoop* pLogicTaskLoop = dynamic_cast<LogicTaskLoop*>(
        TaskLoopManager::getSingleton().createObject(GET_OBJECT_TYPE(LogicTaskLoop), ON_Logic_TaskLoop)
        );
    pLogicTaskLoop->run();
    */

    m_pTimer = U2_NEW Timer;
    m_pMovableSnapshotManager = U2_NEW MovableSnapshotManager;
    m_pControlSnapshotManager = U2_NEW ControlSnapshotManager;
}
//-----------------------------------------------------------------------
Root::~Root()
{
    U2_DELETE m_pTimer;
    U2_DELETE m_pMovableSnapshotManager;
    U2_DELETE m_pControlSnapshotManager;
}
//-----------------------------------------------------------------------
Timer* Root::getTimer()
{
    return m_pTimer;
}