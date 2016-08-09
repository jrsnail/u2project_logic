#include "U2SnapshotDataPool.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void SceneSnapshot::addMovableSnapshot(MovableSnapshot* movable)
{
    MovableSnapshotList::iterator it 
        = std::find(movableSnapshotList.begin(), movableSnapshotList.end(), movable);
    if (it == movableSnapshotList.end())
    {
        movableSnapshotList.push_back(movable);
    }
    else
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SnapshotDataPool::SnapshotDataPool(const String& type, const String& name)
    : DataPool(type, name)
{
}
//-----------------------------------------------------------------------
SnapshotDataPool::~SnapshotDataPool()
{
}
//-----------------------------------------------------------------------
void SnapshotDataPool::addSceneSnapshot(SceneSnapshot* scene)
{
    U2_LOCK_MUTEX(m_SceneSnapshotListMutex);

    SceneSnapshotList::iterator it
        = std::find(m_SceneSnapshotList.begin(), m_SceneSnapshotList.end(), scene);
    if (it == m_SceneSnapshotList.end())
    {
        m_SceneSnapshotList.push_back(scene);
    }
    else
    {
        assert(0);
    }
}