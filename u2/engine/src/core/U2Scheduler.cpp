#include "U2Scheduler.h"

#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2FrameListenerCollection.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SchedulerTask::SchedulerTask(const String& type, const String& name)
    : Task(type, name)
    , m_ulPeriod(0L)
    , m_bRepeat(false)
    , m_bCatchup(true)
{
    
}
//-----------------------------------------------------------------------
SchedulerTask::~SchedulerTask()
{

}
//-----------------------------------------------------------------------
void SchedulerTask::initialize(u2uint64 period
    , bool repeat, bool catchUp)
{
    m_ulPeriod = period;
    m_bRepeat = repeat;
    m_bCatchup = catchUp;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Scheduler::Scheduler(const String& type, const String& name)
    : Object(type, name)
    , m_bCallOnFrameStarted(true)
{
    FrameListenerCollection::getSingleton().addFrameListener(this
        , std::bind(&Scheduler::_onUpdate, this, std::placeholders::_1));
}
//-----------------------------------------------------------------------
Scheduler::~Scheduler()
{
    FrameListenerCollection::getSingleton().removeFrameListener(this);
}
//-----------------------------------------------------------------------
void Scheduler::initialize(bool callOnFrameStarted)
{
    m_bCallOnFrameStarted = callOnFrameStarted;
}
//-----------------------------------------------------------------------
SchedulerTask* Scheduler::createObject(const String& type, const String& name)
{
    SchedulerTask* pObj = SimpleObjectManager<SchedulerTask>::createObject(type, name);
    if (pObj != nullptr)
    {
        // do some init
    }
    return pObj;
}
//-----------------------------------------------------------------------
void Scheduler::destoryObject(SchedulerTask* obj)
{
    assert(obj != nullptr);

    for (SchedulerTaskMap::iterator it = m_SchedulerTaskMap.begin(); 
    it != m_SchedulerTaskMap.end(); it++)
    {
        if (it->second == obj)
        {
            m_SchedulerTaskMap.erase(it);
            break;
        }
    }

    SimpleObjectManager<SchedulerTask>::destoryObject(obj);
}
//-----------------------------------------------------------------------
void Scheduler::_onUpdate(float dt)
{
    if (!m_bCallOnFrameStarted)
        return;
    while (_next());
}
//-----------------------------------------------------------------------
bool Scheduler::_next()
{
    if (m_SchedulerTaskMap.empty())
    {
        return false;
    }
    
    SchedulerTaskMap::iterator it = m_SchedulerTaskMap.begin();
    u2uint64 now = mTimer.getMilliseconds();
    u2uint64 then = it->first;
    //std::cout << " now >= then " << now << " >= " << then << std::endl; 
    if (now >= then)
    {
        SchedulerTask* pTask = it->second; // get a copy of the timer instance 
        m_SchedulerTaskMap.erase(it); // pop the sucker off the map. 
        u2uint64 ulNextTrigger = pTask->trigger(now - then); // trigger it 
        if (ulNextTrigger > 0)
        {
            // if it returns non-zero, put it back in the map. 
            m_SchedulerTaskMap.insert(std::make_pair(now + ulNextTrigger, pTask));
        }
        else
        {
            //SimpleObjectManager<SchedulerTask>::destoryObject(pTask);
            U2_DELETE pTask;
        }
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> SchedulerManager* Singleton<SchedulerManager>::msSingleton = 0;
SchedulerManager* SchedulerManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new SchedulerManager;
    }
    return msSingleton;
}
SchedulerManager& SchedulerManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
SchedulerManager::SchedulerManager()
{
    CREATE_FACTORY(Scheduler);
}
//-----------------------------------------------------------------------
SchedulerManager::~SchedulerManager()
{
}
//-----------------------------------------------------------------------
Scheduler* SchedulerManager::createObject(const String& type, const String& name)
{
    return SimpleObjectManager<Scheduler>::createObject(type, name);
}
//-----------------------------------------------------------------------
Scheduler* SchedulerManager::createObject(const String& type, const String& name
    , bool callOnFrameStarted)
{
    Scheduler* pObj = createObject(type, name);
    if (pObj != nullptr)
    {
        pObj->initialize(callOnFrameStarted);
    }
    return pObj;
}