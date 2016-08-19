#include "U2Task.h"

#include "U2TaskLoop.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
PostTaskAndReplyRelay::PostTaskAndReplyRelay(const String& type, const String& name, const String& guid)
    : Task(GET_OBJECT_TYPE(PostTaskAndReplyRelay), name, guid)
    , m_spOriginLoop(TaskLoopManager::current())
{

}
//---------------------------------------------------------------------
PostTaskAndReplyRelay::~PostTaskAndReplyRelay()
{
}
//---------------------------------------------------------------------
void PostTaskAndReplyRelay::initialize(Task* task, Task* reply)
{
    m_pTask = task;
    m_pReply = reply;
}
//---------------------------------------------------------------------
void PostTaskAndReplyRelay::run()
{
    m_pTask->run();
    m_spOriginLoop->postTask(
        TaskManager::getSingletonPtr()->createObjectWithFunction([=] { this->runReplyAndSelfDestruct(); })
        );
}
//---------------------------------------------------------------------
void PostTaskAndReplyRelay::runReplyAndSelfDestruct()
{
    m_pReply->run();

    TaskManager::getSingletonPtr()->destoryObject(m_pTask);
    TaskManager::getSingletonPtr()->destoryObject(m_pReply);
    TaskManager::getSingletonPtr()->destoryObject(this);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> TaskManager* Singleton<TaskManager>::msSingleton = 0;
TaskManager* TaskManager::getSingletonPtr(void)
{
    return msSingleton;
}
TaskManager& TaskManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
TaskManager::TaskManager()
{
}
//-----------------------------------------------------------------------
TaskManager::~TaskManager()
{
}
//-----------------------------------------------------------------------
Task* TaskManager::createObject(const String& type, const String& name, const String& guid)
{
    U2_LOCK_AUTO_MUTEX;

    Task* pObj = SimpleObjectManager<Task>::createObject(type, name, guid);
    if (pObj != nullptr)
    {
        // do some init
    }
    return pObj;
}
//-----------------------------------------------------------------------
PostTaskAndReplyRelay* TaskManager::createObject(
    const String& type, const String& name, const String& guid, Task* task, Task* reply)
{
    PostTaskAndReplyRelay* pRelay = dynamic_cast<PostTaskAndReplyRelay*>(createObject(type, name, guid));
    if (pRelay != nullptr)
    {
        pRelay->initialize(task, reply);
    }
    return pRelay;
}
//-----------------------------------------------------------------------
void TaskManager::destoryObject(Task* obj)
{
    U2_LOCK_AUTO_MUTEX;

    assert(obj != nullptr);
    assert(obj == SimpleObjectManager<Task>::retrieveObjectByGuid(obj->getGuid()));
    SimpleObjectManager<Task>::destoryObject(obj);
}
//-----------------------------------------------------------------------
void TaskManager::destoryObjectByName(const String& name)
{
    U2_LOCK_AUTO_MUTEX;
    SimpleObjectManager<Task>::destoryObjectByName(name);
}
//-----------------------------------------------------------------------
Task* TaskManager::retrieveObjectByName(const String& name)
{
    U2_LOCK_AUTO_MUTEX;
    return SimpleObjectManager<Task>::retrieveObjectByName(name);
}
//-----------------------------------------------------------------------
Task* TaskManager::retrieveObjectByGuid(const String& guid)
{
    U2_LOCK_AUTO_MUTEX;
    return SimpleObjectManager<Task>::retrieveObjectByGuid(guid);
}
//-----------------------------------------------------------------------
bool TaskManager::hasObjectByName(const String& name)
{
    U2_LOCK_AUTO_MUTEX;
    return SimpleObjectManager<Task>::hasObjectByName(name);
}
//-----------------------------------------------------------------------
void TaskManager::addObject(Task* obj)
{
    U2_LOCK_AUTO_MUTEX;
    return SimpleObjectManager<Task>::addObject(obj);
}
//-----------------------------------------------------------------------
void TaskManager::removeObject(Task* obj)
{
    U2_LOCK_AUTO_MUTEX;
    SimpleObjectManager<Task>::removeObject(obj);
}