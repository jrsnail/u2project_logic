#ifndef __U2Root_H__
#define __U2Root_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Singleton.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class LogManager;
class ArchiveManager;
class ResourceGroupManager;
class Timer;
class MovableSnapshotManager;
class ControlSnapshotManager;
class ComponentManager;
class GameObjectManager;
class DataPoolManager;
class ScriptManager;
class FrameListenerCollection;
class TaskManager;
class TaskLoopManager;
class FactoryManager;


class Root : public Singleton < Root >, public GeneralAllocatedObject
{
public:
    Root();
    virtual ~Root();

    Timer* getTimer();

    virtual void enter();
    virtual void exit();
    virtual void pause();
    virtual void resume();

protected:
    virtual void _initialize();
    void _loadResources();

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
    static Root& getSingleton(void);

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
    static Root* getSingletonPtr(void);

protected:
    LogManager*					m_pLogManager;
    FactoryManager*             m_pFactoryManager;
    ArchiveManager*             m_pArchiveManager;
    ResourceGroupManager*       m_pResourceGroupManager;
    ComponentManager*		    m_pComponentManager;
    GameObjectManager*          m_pGameObjectManager;
    Timer*                      m_pTimer;
    MovableSnapshotManager*     m_pMovableSnapshotManager;
    ControlSnapshotManager*     m_pControlSnapshotManager;
    DataPoolManager*            m_pDataPoolManager;
    ScriptManager*              m_pScriptManager;
    FrameListenerCollection*    m_pFrameListenerCollection;
    TaskManager*                m_pTaskManager;
    TaskLoopManager*            m_pTaskLoopManager;
};


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif
