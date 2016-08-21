#ifndef __U2Snapshot__
#define __U2Snapshot__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2PoolingObjectManager.h"
#include "cocos2d.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class GameObject;


class MovableSnapshot : public ReusableObject
{
public:
    MovableSnapshot(const String& type, const String& name = BLANK, const String& guid = BLANK)
        : ReusableObject(type, name, guid)
        , ulTimestampOnRequest(0)
        , ulTimestamp(0)
        , bAlive(true)
        , v2Velocity(cocos2d::Vec2::ZERO)
        , v2Position(cocos2d::Vec2::ZERO)
        , uCalCounter(0)
    {

    }

    MovableSnapshot& operator=(const MovableSnapshot& ths)
    {
        szGameObjGuid = ths.szGameObjGuid;
        szGameObjType = ths.szGameObjType;
        ulTimestampOnRequest = ths.ulTimestampOnRequest;
        ulTimestamp = ths.ulTimestamp;
        bAlive = ths.bAlive;
        v2Velocity = ths.v2Velocity;
        v2Position = ths.v2Position;
        uCalCounter = ths.uCalCounter;
        return *this;
    }

    MovableSnapshot& operator=(MovableSnapshot&& ths)
    {
        szGameObjGuid = ths.szGameObjGuid;
        szGameObjType = ths.szGameObjType;
        ulTimestampOnRequest = ths.ulTimestampOnRequest;
        ulTimestamp = ths.ulTimestamp;
        bAlive = ths.bAlive;
        v2Velocity = ths.v2Velocity;
        v2Position = ths.v2Position;
        uCalCounter = ths.uCalCounter;
        return *this;
    }

    virtual void preRecycleByPool() override 
    {
        ReusableObject::preRecycleByPool();

        szGameObjGuid = BLANK;
        szGameObjType = BLANK;
        ulTimestampOnRequest = 0L;
        ulTimestamp = 0L;
        bAlive = true;
        v2Velocity = cocos2d::Vec2::ZERO;
        v2Position = cocos2d::Vec2::ZERO;
        uCalCounter = 0;
    };
    virtual void postReuseFromPool() override 
    {
        ReusableObject::postReuseFromPool();

        szGameObjGuid = BLANK;
        szGameObjType = BLANK;
        ulTimestampOnRequest = 0L;
        ulTimestamp = 0L;
        bAlive = true;
        v2Velocity = cocos2d::Vec2::ZERO;
        v2Position = cocos2d::Vec2::ZERO;
        uCalCounter = 0;
    };

    u2::String      szGameObjGuid;
    u2::String      szGameObjType;
    u2uint64        ulTimestampOnRequest;
    u2uint64        ulTimestamp;
    bool            bAlive;
    cocos2d::Vec2   v2Velocity;
    cocos2d::Vec2   v2Position;
    u2uint32        uCalCounter;
};


class MovableSnapshotManager : public Singleton < MovableSnapshotManager >
    , public PoolingObjectManager<MovableSnapshot>, public GeneralAllocatedObject
{
public:
    MovableSnapshotManager();
    virtual ~MovableSnapshotManager();

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static MovableSnapshotManager& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static MovableSnapshotManager* getSingletonPtr(void);

protected:
};


class ControlSnapshot : public ReusableObject
{
public:
    ControlSnapshot(const String& type, const String& name = BLANK, const String& guid = BLANK)
        : ReusableObject(type, name, guid)
        , ulTimestampOnRequest(0)
        , v2Velocity(cocos2d::Vec2::ZERO)
    {

    }

    ControlSnapshot& operator=(const ControlSnapshot& ths)
    {
        szPlayerGuid = ths.szPlayerGuid;
        szGameObjGuid = ths.szGameObjGuid;
        szGameObjType = ths.szGameObjType;
        ulTimestampOnRequest = ths.ulTimestampOnRequest;
        v2Velocity = ths.v2Velocity;
        return *this;
    }

    ControlSnapshot& operator=(ControlSnapshot&& ths)
    {
        szPlayerGuid = ths.szPlayerGuid;
        szGameObjGuid = ths.szGameObjGuid;
        szGameObjType = ths.szGameObjType;
        ulTimestampOnRequest = ths.ulTimestampOnRequest;
        v2Velocity = ths.v2Velocity;
        return *this;
    }

    virtual void preRecycleByPool() override
    {
        ReusableObject::preRecycleByPool();

        szPlayerGuid = BLANK;
        szGameObjGuid = BLANK;
        szGameObjType = BLANK;
        ulTimestampOnRequest = 0L;
        v2Velocity = cocos2d::Vec2::ZERO;
    };

    virtual void postReuseFromPool() override
    {
        ReusableObject::postReuseFromPool();

        szPlayerGuid = BLANK;
        szGameObjGuid = BLANK;
        szGameObjType = BLANK;
        ulTimestampOnRequest = 0L;
        v2Velocity = cocos2d::Vec2::ZERO;
    };


    u2::String      szPlayerGuid;
    u2::String      szGameObjGuid;
    u2::String      szGameObjType;
    u2uint64        ulTimestampOnRequest;
    cocos2d::Vec2   v2Velocity;
};


class ControlSnapshotManager : public Singleton < ControlSnapshotManager >
    , public PoolingObjectManager<ControlSnapshot>, public GeneralAllocatedObject
{
public:
    ControlSnapshotManager();
    virtual ~ControlSnapshotManager();

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static ControlSnapshotManager& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static ControlSnapshotManager* getSingletonPtr(void);

protected:
};


/** Not thread safe, should be in single thread.
*/
class Scene : public Singleton<Scene>, public GeneralAllocatedObject
{
public:
    /// <GameObj Guid, MovableSnapshot*>
    typedef map< String, MovableSnapshot* >::type                          FrameSnapshot;
    /// <timestamp, MovableSnapshot*>
    typedef map< u2uint64, MovableSnapshot*, std::less<u2uint64> >::type   MoveableSnapshotMap;
    /// <GameObj Guid, MoveableSnapshotMap>
    typedef map< String, MoveableSnapshotMap >::type                       GameObjMoveableSnapshotMap;
    typedef map< u2uint64, ControlSnapshot*, std::less<u2uint64> >::type   ControlSnapshotMap;

    /// <GameObj Guid, GameObject*>
    typedef map< String, GameObject* >::type                               GameObjectMap;

public:
    Scene();
    virtual ~Scene();

    void addFrameSnapshot(FrameSnapshot* frame);

    void eraseMovableSnapshotByGameObjGuid(const String& guid);

    const MoveableSnapshotMap& retrieveAllMoveableSnapshotsByGameObjGuid(const String& guid) const;

    MoveableSnapshotMap& retrieveAllMoveableSnapshotsByGameObjGuid(const String& guid);

    void pushbackControlSnapshot(ControlSnapshot* control);

    ControlSnapshot* retrieveControlSnapshotOnTimestamp(u2uint64 timestamp);

    void eraseControlSnapshotBeforeTimestamp(u2uint64 timestamp);

    const ControlSnapshotMap& retrieveAllControlSnapshots();

    virtual void start();

    virtual void end();

    GameObject* createGameObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

    void destroyGameObject(GameObject* gameObj);

protected:
    virtual void _updateGameObjWithSnapshot();
    virtual void _onUpdate(float dt);
    virtual void _initGameObj(GameObject* gameObj, MovableSnapshot* movableSnapshot) = 0;

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static Scene& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static Scene* getSingletonPtr(void);

protected:
    GameObjMoveableSnapshotMap m_Scene;
    ControlSnapshotMap m_ControlSnapshotMap;
    GameObjectMap               m_GameObjectMap;
};



U2EG_NAMESPACE_END


#include "U2HeaderSuffix.h"


#endif /* defined(__U2Snapshot__) */
