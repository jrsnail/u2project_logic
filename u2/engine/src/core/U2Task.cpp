#include "U2Task.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
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
    Task* pObj = createObject(type, name);
    if (pObj != nullptr)
    {
        // do some init
    }
    return pObj;
}
//-----------------------------------------------------------------------
Task* TaskManager::createObject(const String& type, const String& name)
{
    return SimpleObjectManager<Task>::createObject(type, name);
}
//-----------------------------------------------------------------------
void TaskManager::destoryObject(Task* obj)
{
    assert(obj != nullptr);
    assert(obj == SimpleObjectManager<Task>::retrieveObjectByGuid(obj->getGuid()));
    SimpleObjectManager<Task>::destoryObject(obj);
}