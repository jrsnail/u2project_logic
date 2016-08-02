//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2LuaScript__
#define __U2LuaScript__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2FactoryObj.h"
#include "U2Script.h"
#include "U2SimpleObjectManager.h"
#include "U2Task.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


U2EG_NAMESPACE_BEGIN


class LuaTask : public Task
{
public:
    LuaTask(const String& type, const String& name);

    virtual ~LuaTask();

    // Tasks are automatically deleted after Run is called.  
    virtual void run() override;

    inline void setLuaScriptName(const String& scriptName);
    inline const String& getLuaScriptName() const;

    void setField(const String& key, const String& value);
    bool hasField(const String& key) const;
    const String& getField(const String& key) const;

protected:
    typedef std::map<String, String>  ParamMap;
    ParamMap    m_Params;

    String m_szScriptName;
};


#define CREATE_LUATASK_FACTORY(TypeStr)                                                         \
    if (!u2::FactoryManager::getSingletonPtr()->hasObjectFactory(TypeStr))                      \
    {                                                                                           \
        u2::ObjectFactory* pObjectFactory = new u2::TemplateObjectFactory<u2::LuaTask>(TypeStr);\
        u2::FactoryManager::getSingletonPtr()->addObjectFactory(pObjectFactory);                \
    }


class LuaScript : public u2::Script
{
protected:
    enum eLuaScriptState
    {
        LSS_WAITFRAME,
        LSS_WAITTIME,
        LSS_RUNNING,
        LSS_NOTLOADED,
        LSS_DONE
    };

public:
    LuaScript(const String& type, const String& name);
    virtual ~LuaScript();

    void addNotification(const String& notification);
    
    virtual Script::NotificationNames listNotificationInterests(void) override;
    
    virtual void handleNotification(const Notification& notification) override;
    
    virtual void onRegister(void) override;
    
    virtual void onRemove(void) override;

    virtual void initialize(lua_State* mainLuaState = nullptr);

    inline lua_State* getLuaState() const;

    void changeStackIndex();

    virtual void runFile(const String& fileName) override;
    virtual int runString(const String& luaString) override;
    virtual int runGlobalFunc(const String& fnName);
    virtual int _executeFunction(int numArgs);
    virtual void resumeScript(float param);
    virtual void onUpdate(float elapsedSec);
    virtual void onProcessScriptTask(float elapsedSec);
    LuaTask* createLuaTask(const String& type, const String& name);

    void pushC2LTask(Task* task);
    Task* frontC2LTask();
    void popC2LTask();
    void destroyC2LTask(Task* task);

    void pushL2CTask(Task* task);
    Task* frontL2CTask();
    void popL2CTask();

protected:
    void run();
    void _outputError();

protected:
    lua_State*      m_pThreadState;
    int             m_nIndexOnStack;
    eLuaScriptState m_eState;              // are we waiting, not loaded yet, etc.
    float           m_fWaitTimestamp;      // time to wake up
    int             m_nWaitFrame;          // number of frames to wait
    float           m_fCurrentTime;        // current time
    bool            m_bLoadedFile;
    std::list<Task*>    m_C2LQueue;
    std::list<Task*>    m_L2CQueue;
    NotificationNames   m_NotificationNames;
};


class LuaScriptManager : public ScriptManager
{
public:
    /** Default constructor - should never get called by a client app.
    */
    LuaScriptManager();

    /** Default destructor.
    */
    virtual ~LuaScriptManager();

    virtual Script* createObject(const String& type, const String& name = BLANK) override;

    virtual void destoryObject(Script* obj);

    LuaScript* retrieveObjectByLuaState(lua_State*);

    inline lua_State* getMainLuaState() const;
    
protected:
    lua_State*      m_pMainThread;

    typedef std::map<lua_State*, String>           LuaStateNameMap;
    LuaStateNameMap m_LuaStateNameMap;

    typedef std::list<LuaScript*>                       LuaScriptList;
    LuaScriptList   m_LuaScriptList;

};


U2EG_NAMESPACE_END



#endif /* defined(__U2Object__) */
