#ifndef __U2SnapshotDataPool__
#define __U2SnapshotDataPool__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2DataPool.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class MovableSnapshot : public GeneralAllocatedObject
{
public:
    MovableSnapshot()
        : ulTimestamp(0)
        , bAlive(true)
        , v2Velocity(cocos2d::Vec2::ZERO)
        , v2Position(cocos2d::Vec2::ZERO)
    {

    }

    u2::String      szGameObjGuid;
    u2uint64        ulTimestamp;
    bool            bAlive;
    cocos2d::Vec2   v2Velocity;
    cocos2d::Vec2   v2Position;
};


class SceneSnapshot : public GeneralAllocatedObject
{
public:
    typedef vector<MovableSnapshot*>::type      MovableSnapshotList;

public:
    SceneSnapshot()
        : ulTimestamp(0)
    {

    }

    void addMovableSnapshot(MovableSnapshot* movable);

    typedef VectorIterator<MovableSnapshotList>          MovableSnapshotListIterator;
    typedef ConstVectorIterator<MovableSnapshotList>     ConstMovableSnapshotListIterator;
    MovableSnapshotListIterator retrieveAllMovableSnapshots(const String& type)
    {
        return MovableSnapshotListIterator(
            movableSnapshotList.begin(), movableSnapshotList.end());
    }

    u2uint64        ulTimestamp;
    MovableSnapshotList movableSnapshotList;
};


class SnapshotDataPool : public DataPool
{
public:
    typedef list<SceneSnapshot*>::type      SceneSnapshotList;

public:
    SnapshotDataPool(const String& type, const String& name = BLANK, const u2::String& guid = BLANK);
    virtual ~SnapshotDataPool();

    void addSceneSnapshot(SceneSnapshot* scene);

    typedef VectorIterator<SceneSnapshotList>          SceneSnapshotListIterator;
    typedef ConstVectorIterator<SceneSnapshotList>     ConstSceneSnapshotListIterator;
    // todo: it is not a thread safe method!!
    SceneSnapshotListIterator retrieveAllSceneSnapshots()
    {
        return SceneSnapshotListIterator(
            m_SceneSnapshotList.begin(), m_SceneSnapshotList.end());
    }

protected:
    U2_MUTEX(m_SceneSnapshotListMutex);
    SceneSnapshotList m_SceneSnapshotList;
};



static SnapshotDataPool* SNAPSHOTDATAPOOL(String tableName)
{
    SnapshotDataPool* pDataPool = dynamic_cast<SnapshotDataPool*>(
        DataPoolManager::getSingleton().retrieveObjectByName(tableName));
    if (pDataPool == nullptr)
    {
        assert(0);
    }
    return pDataPool;
}


U2EG_NAMESPACE_END


#include "U2HeaderSuffix.h"


#endif /* defined(__U2SnapshotDataPool__) */
