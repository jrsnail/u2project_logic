#include "U2Task.h"

#include "U2TaskLoop.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
PostTaskAndReplyRelay::PostTaskAndReplyRelay(const std::string& type, const std::string& name)
    : Task(GET_OBJECT_TYPE(PostTaskAndReplyRelay), name)
    , m_spOriginLoop(MsgLoopManager::current())
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
        TaskManager::getSingletonPtr()->createObject([=] { this->runReplyAndSelfDestruct(); })
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
    if (msSingleton == nullptr)
    {
        msSingleton = new TaskManager;
    }
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
Task* TaskManager::createObject(const String& type, const String& name)
{
    Task* pObj = SimpleObjectManager<Task>::createObject(type, name);
    if (pObj != nullptr)
    {
        // do some init
    }
    return pObj;
}
//-----------------------------------------------------------------------
PostTaskAndReplyRelay* TaskManager::createObject(
    const String& type, const String& name, Task* task, Task* reply)
{
    PostTaskAndReplyRelay* pRelay = dynamic_cast<PostTaskAndReplyRelay*>(createObject(type, name));
    if (pRelay != nullptr)
    {
        pRelay->initialize(task, reply);
    }
    return pRelay;
}
//-----------------------------------------------------------------------
void TaskManager::destoryObject(Task* obj)
{
    assert(obj != nullptr);
    assert(obj == SimpleObjectManager<Task>::retrieveObjectByGuid(obj->getGuid()));
    SimpleObjectManager<Task>::destoryObject(obj);
}