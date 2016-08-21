#include "U2Root.h"

#include "U2Timer.h"
#include "U2SnapshotDataPool.h";



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