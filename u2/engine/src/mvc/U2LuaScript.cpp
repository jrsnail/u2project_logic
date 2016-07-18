//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2LuaScript.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "U2FrameListenerCollection.h"
#include "U2PredefinedPrerequisites.h"
#include "U2Facade.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
LuaTask::LuaTask(const String& type, const String& name)
: Task(type, name)
{
}
//-----------------------------------------------------------------------
LuaTask::~LuaTask()
{
}
//-----------------------------------------------------------------------
void LuaTask::run()
{

}
//-----------------------------------------------------------------------
void LuaTask::setLuaScriptName(const String& scriptName)
{
    m_szScriptName = scriptName;
}
//-----------------------------------------------------------------------
const String& LuaTask::getLuaScriptName() const
{
    return m_szScriptName;
}
//-----------------------------------------------------------------------
void LuaTask::setField(const String& key, const String& value)
{
    m_Params[key] = value;
}
//-----------------------------------------------------------------------
bool LuaTask::hasField(const String& key) const
{
    ParamMap::const_iterator it = m_Params.find(key);
    return it != m_Params.end();
}
//-----------------------------------------------------------------------
const String& LuaTask::getField(const String& key) const
{
    ParamMap::const_iterator it = m_Params.find(key);
    if (it != m_Params.end())
    {
        return it->second;
    }
    else
    {
        assert(0);
        static String str(BLANK);
        return str;
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
LuaScript::LuaScript(const String& type, const String& name)
    : Script(type, name)
    , m_nIndexOnStack(0)
    , m_eState(LSS_NOTLOADED)
    , m_fWaitTimestamp(0.0f)
    , m_nWaitFrame(0)
    , m_fCurrentTime(0.0f)
    , m_bLoadedFile(false)
{

}
//-----------------------------------------------------------------------
LuaScript::~LuaScript()
{
    if (nullptr != m_pThreadState)
    {
        //lua_close(m_pThreadState);
        lua_remove(((LuaScriptManager*)LuaScriptManager::getSingletonPtr())->getMainLuaState(), m_nIndexOnStack);
        m_pThreadState = nullptr;

    }
}
//-----------------------------------------------------------------------
void LuaScript::addNotification(const String& notification)
{
    for (NotificationNames::const_iterator it = m_NotificationNames.begin();
        it != m_NotificationNames.end(); it++)
    {
        if ((*it) == notification)
        {
            assert(0);
            return;
        }
    }
    m_NotificationNames.push_back(notification);
}
//-----------------------------------------------------------------------
Script::NotificationNames LuaScript::listNotificationInterests(void)
{
    return m_NotificationNames;
}
//-----------------------------------------------------------------------
void LuaScript::handleNotification(const Notification& notification)
{
}
//-----------------------------------------------------------------------
void LuaScript::onRegister(void)
{
    this->run();
}
//-----------------------------------------------------------------------
void LuaScript::onRemove(void)
{
    FrameListenerCollection::getSingletonPtr()->removeFrameListener(this);
}
//-----------------------------------------------------------------------
void LuaScript::initialize(lua_State* mainLuaState)
{
    // create main lua state
    if (mainLuaState == nullptr)
    {
        assert(0);
    }
    else
    {
        // create a thread/state for this object
        m_pThreadState = lua_newthread(mainLuaState);
        m_nIndexOnStack = lua_gettop(mainLuaState);

        luaL_openlibs(m_pThreadState);
    }
}
//-----------------------------------------------------------------------
lua_State* LuaScript::getLuaState() const
{
    return m_pThreadState;
}
//-----------------------------------------------------------------------
void LuaScript::changeStackIndex()
{
    m_nIndexOnStack -= 1;
}
//-----------------------------------------------------------------------
void LuaScript::runFile(const String& fileName)
{
    if (m_bLoadedFile)
    {
        return;
    }
    assert(((LuaScriptManager*)ScriptManager::getSingletonPtr())->getMainLuaState());
    assert(m_pThreadState);

    // here we should use luaL_loadbuffer function as lua file may 
    // place on virtual file system
    int nStatus = luaL_loadfile(m_pThreadState, fileName.c_str());
    if (nStatus == 0)
    {
        m_bLoadedFile = true;
    }
    else
    {
        _outputError();
    }
}
//-----------------------------------------------------------------------
int LuaScript::runString(const String& luaString)
{
    assert(((LuaScriptManager*)ScriptManager::getSingletonPtr())->getMainLuaState());
    assert(m_pThreadState);

    int nStatus = luaL_loadbuffer(m_pThreadState, luaString.c_str(), luaString.size(), "Console");
    if (nStatus == 0)
    {
        nStatus = lua_pcall(m_pThreadState, lua_gettop(m_pThreadState) - 1, 0, 0);
    }

    if (nStatus)
    {
        _outputError();
        return(-1);
    }

    return(0);
}
//-----------------------------------------------------------------------
int LuaScript::runGlobalFunc(const String& fnName)
{
    // find the lua function and push it on the stack
    lua_getglobal(m_pThreadState, fnName.c_str());

    if (!lua_isfunction(m_pThreadState, -1))
    {
        CCLOG("[LUA ERROR]: [%s] is not function", fnName.c_str());
        lua_pop(m_pThreadState, 1);
        return 0;
    }

    return _executeFunction(0);
}
//-----------------------------------------------------------------------
int LuaScript::_executeFunction(int numArgs)
{
    int functionIndex = -(numArgs + 1);
    if (!lua_isfunction(m_pThreadState, functionIndex))
    {
        CCLOG("[LUA ERROR]: [%d] is not function", functionIndex);
        lua_pop(m_pThreadState, numArgs + 1); // remove function and arguments
        return 0;
    }

    int traceback = 0;
    lua_getglobal(m_pThreadState, "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
    if (!lua_isfunction(m_pThreadState, -1))
    {
        lua_pop(m_pThreadState, 1);                                            /* L: ... func arg1 arg2 ... */
    }
    else
    {
        lua_insert(m_pThreadState, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
        traceback = functionIndex - 1;
    }

    int error = lua_pcall(m_pThreadState, numArgs, 1, traceback);              /* L: ... [G] ret */
    if (error)
    {
        if (traceback == 0)
        {
            CCLOG("[LUA ERROR]: %s", lua_tostring(m_pThreadState, -1));        /* L: ... error */
            lua_pop(m_pThreadState, 1); // remove error message from stack
        }
        else                                                            /* L: ... G error */
        {
            lua_pop(m_pThreadState, 2); // remove __G__TRACKBACK__ and error message from stack
        }
        return 0;
    }

    // get return value
    int ret = 0;
    if (lua_isnumber(m_pThreadState, -1))
    {
        ret = (int)lua_tointeger(m_pThreadState, -1);
    }
    else if (lua_isboolean(m_pThreadState, -1))
    {
        ret = (int)lua_toboolean(m_pThreadState, -1);
    }
    // remove return value from stack
    lua_pop(m_pThreadState, 1);                                                /* L: ... [G] */

    if (traceback)
    {
        lua_pop(m_pThreadState, 1); // remove __G__TRACKBACK__ from stack      /* L: ... */
    }

    return ret;
}
//-----------------------------------------------------------------------
void LuaScript::resumeScript(float param)
{
    // we're about to run/resume the thread, so set the global
    int nStatus = LSS_RUNNING;

    // param is treated as a return value from the function that yielded
    //lua_pushnumber(m_pThreadState, param);

    nStatus = lua_resume(m_pThreadState, 0);

    if (nStatus)
    {
        _outputError();
    }
}
//-----------------------------------------------------------------------
void LuaScript::_outputError()
{
    const char* msg = lua_tostring(m_pThreadState, -1);
    if (msg == nullptr)
    {
        msg = "(error with no message)";
    }
    lua_pop(m_pThreadState, 1);

    CCLOG("[LUA ERROR]: %s", msg);
}
//-----------------------------------------------------------------------
void LuaScript::run()
{
    onProcessScriptTask(0.0f);
    FrameListenerCollection::getSingletonPtr()->addFrameListener(
        this, std::bind(&LuaScript::onProcessScriptTask, this, std::placeholders::_1)
        );
}
//-----------------------------------------------------------------------
void LuaScript::onUpdate(float elapsedSec)
{
    m_fCurrentTime += elapsedSec;

    switch (m_eState)
    {
    case LSS_WAITTIME:
        if (m_fCurrentTime >= m_fWaitTimestamp)
        {
            resumeScript(0.0f);
        }
        break;
    case LSS_WAITFRAME:
        m_nWaitFrame--;
        if (m_nWaitFrame <= 0)
        {
            resumeScript(0.0f);
        }
        break;
    case LSS_NOTLOADED:
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------
void LuaScript::onProcessScriptTask(float elapsedSec)
{
    // resume lua coroutine, and pause c
    int nStatus = lua_resume(m_pThreadState, 0);
    if (LUA_YIELD == nStatus || 0 == nStatus)
    {
        // run l2c task
        while (Task* pTask = frontL2CTask())
        {
            pTask->run();
            // destroy task
            popL2CTask();
        }
    }

    // error
    if (LUA_YIELD != nStatus && 0 != nStatus)
    {
        // todo: error report
        _outputError();
//         getFacade().removeScript(getName());
//         LuaScriptManager::getSingletonPtr()->destoryObject(this);
    }

    // normal exit
    if (0 == nStatus)
    {
        getFacade().removeScript(getName());
        LuaScriptManager::getSingletonPtr()->destoryObject(this);
    }
}
//-----------------------------------------------------------------------
LuaTask* LuaScript::createLuaTask(const String& type, const String& name)
{
    Task* pTask = TaskManager::getSingletonPtr()->createObject(type, name);
    LuaTask* pLuaTask = dynamic_cast<LuaTask*>(pTask);
    if (pLuaTask != nullptr)
    {
        pLuaTask->setLuaScriptName(getName());
    }
    return pLuaTask;
}
//-----------------------------------------------------------------------
void LuaScript::pushC2LTask(Task* task)
{
    m_C2LQueue.push_back(task);
}
//-----------------------------------------------------------------------
Task* LuaScript::frontC2LTask()
{
    if (m_C2LQueue.size() == 0)
    {
        return nullptr;
    }
    else
    {
        return *(m_C2LQueue.begin());
    }
}
//-----------------------------------------------------------------------
void LuaScript::popC2LTask()
{
    std::list<Task*>::iterator it = m_C2LQueue.begin();
    if (it != m_C2LQueue.end())
    {
        TaskManager::getSingletonPtr()->destoryObject(*it);
        m_C2LQueue.erase(it);
    }
}
//-----------------------------------------------------------------------
void LuaScript::destroyC2LTask(Task* task)
{
    std::list<Task*>::iterator it = std::find(m_C2LQueue.begin(), m_C2LQueue.end(), task);
    if (it != m_C2LQueue.end())
    {
        TaskManager::getSingletonPtr()->destoryObject(*it);
        m_C2LQueue.erase(it);
    }
}
//-----------------------------------------------------------------------
void LuaScript::pushL2CTask(Task* task)
{
    std::list<Task*>::iterator it = std::find(m_L2CQueue.begin(), m_L2CQueue.end(), task);
    if (it == m_L2CQueue.end())
    {
        m_L2CQueue.push_back(task);
    }
    else
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
Task* LuaScript::frontL2CTask()
{
    if (m_L2CQueue.size() > 0)
    {
        return *(m_L2CQueue.begin());
    }
    return nullptr;
}
//-----------------------------------------------------------------------
void LuaScript::popL2CTask()
{
    std::list<Task*>::iterator it = m_L2CQueue.begin();
    if (it != m_L2CQueue.end())
    {
        TaskManager::getSingletonPtr()->destoryObject(*it);
        m_L2CQueue.erase(it);
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
LuaScriptManager::LuaScriptManager() 
    : ScriptManager()
    , m_pMainThread(nullptr)
{
    CREATE_FACTORY(LuaScript);

    // create main thread/state for this object
    m_pMainThread = lua_open();
    luaL_openlibs(m_pMainThread);
}
//-----------------------------------------------------------------------
LuaScriptManager::~LuaScriptManager()
{
    if (nullptr != m_pMainThread)
    {
        lua_close(m_pMainThread);
        m_pMainThread = nullptr;
    }
}
//-----------------------------------------------------------------------
Script* LuaScriptManager::createObject(const String& type, const String& name)
{
    // As simple object manager, we index object with name which should be an unique key
    assert(retrieveObjectByName(name) == nullptr);
    
    LuaScript* pObj = dynamic_cast<LuaScript*>(ObjectCollection::getSingletonPtr()->createObject(type, name));
    if (pObj != nullptr)
    {
        pObj->initialize(m_pMainThread);
        m_NamedMap[pObj->getName()] = pObj;
        m_LuaStateNameMap[pObj->getLuaState()] = pObj->getName();
        m_LuaScriptList.push_back(pObj);
    }
    return pObj;
}
//-----------------------------------------------------------------------
void LuaScriptManager::destoryObject(Script* obj)
{
    LuaScript* pLuaScript = dynamic_cast<LuaScript*>(obj);
    if (pLuaScript == nullptr)
    {
        assert(0);
    }

    // erase from LuaStateNameMap
    LuaStateNameMap::iterator it = m_LuaStateNameMap.find(pLuaScript->getLuaState());
    if (it == m_LuaStateNameMap.end())
    {
        assert(0);
    }
    else
    {
        m_LuaStateNameMap.erase(it);
    }

    bool bFound = false;
    for (LuaScriptList::iterator it = m_LuaScriptList.begin(); it != m_LuaScriptList.end(); it++)
    {
        if (bFound)
        {
            (*it)->changeStackIndex();
        }
        else
        {
            if (*it == pLuaScript)
            {
                bFound = true;
            }
        }
    }


    // destroy obj
    ScriptManager::destoryObject(obj);
}
//-----------------------------------------------------------------------
LuaScript* LuaScriptManager::retrieveObjectByLuaState(lua_State* state)
{
    LuaStateNameMap::iterator it = m_LuaStateNameMap.find(state);
    if (it == m_LuaStateNameMap.end())
    {
        return nullptr;
    }
    else
    {
        return dynamic_cast<LuaScript*>(retrieveObjectByName(it->second));
    }
}
//-----------------------------------------------------------------------
lua_State* LuaScriptManager::getMainLuaState() const
{
    return m_pMainThread;
}