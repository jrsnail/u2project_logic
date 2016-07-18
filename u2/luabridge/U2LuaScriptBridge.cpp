//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2LuaScriptBridge.h"

#include "U2GameBase.h"
#include "application/AppPrerequisites.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
int registerLuaScriptWithInterestedNotification_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        assert(0);
    }

    u2::LuaScript* pLuaScript = ((u2::LuaScriptManager*)u2::ScriptManager::getSingletonPtr())->retrieveObjectByLuaState(L);
    if (pLuaScript == nullptr)
    {
        assert(0);
    }

    Herald* pHerald = HeraldManager::getSingletonPtr()->retrieveObjectByName(ON_Facade_App);
    if (pHerald != nullptr)
    {
        luaL_checktype(L, 1, LUA_TTABLE);
        size_t n = lua_objlen(L, 1);
        for (size_t i = 1; i <= n; i++)
        {
            lua_rawgeti(L, 1, i);
            std::string szNotificationName = lua_tostring(L, -1);
            lua_pop(L, 1);
            Observer *observer = ObserverManager::getSingletonPtr()->createOrReuseObserver(OT_Observer
                , std::bind(&Script::handleNotification, pLuaScript, std::placeholders::_1)
                , pLuaScript);
            pHerald->registerObserver(szNotificationName, observer);
            pLuaScript->addNotification(szNotificationName);
        }
    }

    return 0;
}
//-----------------------------------------------------------------------
int frontC2LTask_lua(lua_State* L)
{
    u2::LuaScript* pLuaScript = ((u2::LuaScriptManager*)u2::ScriptManager::getSingletonPtr())->retrieveObjectByLuaState(L);
    if (pLuaScript == nullptr)
    {
        assert(0);
        return 0;
    }
    else
    {
        Task* pTask = pLuaScript->frontC2LTask();
        if (pTask == nullptr)
        {
            lua_pushnil(L);
            return 1;
        }
        else
        {
            // create a pointer as lua userdata
            intptr_t* pPointer = (intptr_t*)lua_newuserdata(L, sizeof(intptr_t));
            // point to task address
            *pPointer = (intptr_t)pTask;

            luaL_getmetatable(L, Meta_Name_LuaTask);
            lua_setmetatable(L, -2);

            return 1;
        }
    }
}
//-----------------------------------------------------------------------
int destroyC2LTask_lua(lua_State* L)
{
    u2::LuaScript* pLuaScript = ((u2::LuaScriptManager*)u2::ScriptManager::getSingletonPtr())->retrieveObjectByLuaState(L);
    if (pLuaScript == nullptr)
    {
        assert(0);
        return 0;
    }
    else
    {
        intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 1, Meta_Name_LuaTask);
        pLuaScript->destroyC2LTask((Task*)(*pPointer));
        return 0;
    }
}
//-----------------------------------------------------------------------
int createL2CTask_lua(lua_State* L)
{
    u2::LuaScript* pLuaScript = ((u2::LuaScriptManager*)u2::ScriptManager::getSingletonPtr())->retrieveObjectByLuaState(L);
    if (pLuaScript == nullptr)
    {
        assert(0);
        return 0;
    }
    else
    {
        // check arg count
        int argc = lua_gettop(L);
        if (argc < 1 || argc > 2)
        {
            assert(0);
        }
        // arg 1, task type
        std::string szType = luaL_checkstring(L, 1);
        // arg 2, task name, which ignorable
        std::string szName = BLANK;
        if (argc == 2)
        {
            szName = luaL_checkstring(L, 2);
        }

        // create a pointer as lua userdata
        intptr_t* pPointer = (intptr_t*)lua_newuserdata(L, sizeof(intptr_t));
        // point to task address
        *pPointer = (intptr_t)pLuaScript->createLuaTask(szType, szName);

        luaL_getmetatable(L, Meta_Name_LuaTask);
        lua_setmetatable(L, -2);
        return 1;
    }
}
//-----------------------------------------------------------------------
int pushL2CTask_lua(lua_State* L)
{
    u2::LuaScript* pLuaScript = ((u2::LuaScriptManager*)u2::ScriptManager::getSingletonPtr())->retrieveObjectByLuaState(L);
    if (pLuaScript == nullptr)
    {
        assert(0);
        return 0;
    }
    else
    {
        intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 1, Meta_Name_LuaTask);
        pLuaScript->pushL2CTask((Task*)(*pPointer));
        return 0;
    }
}
//-----------------------------------------------------------------------
int getLuaTaskType_lua(lua_State* L)
{
    intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 1, Meta_Name_LuaTask);
    u2::LuaTask* pTask = (u2::LuaTask*)(*pPointer);
    if (pTask == nullptr)
    {
        assert(0);
        return 0;
    }
    else
    {
        lua_pushstring(L, pTask->getType().c_str());
        return 1;
    }
}
//-----------------------------------------------------------------------
int getLuaTaskName_lua(lua_State* L)
{
    intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 1, Meta_Name_LuaTask);
    u2::LuaTask* pTask = (u2::LuaTask*)(*pPointer);
    if (pTask == nullptr)
    {
        assert(0);
        return 0;
    }
    else
    {
        lua_pushstring(L, pTask->getName().c_str());
        return 1;
    }
}
//-----------------------------------------------------------------------
int getLuaTaskField_lua(lua_State* L)
{
    // get task pointer
    intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 1, Meta_Name_LuaTask);
    u2::LuaTask* pTask = (u2::LuaTask*)(*pPointer);
    if (pTask == nullptr)
    {
        assert(0);
        return 0;
    }
    else
    {
        // get field name
        std::string szFieldName = lua_tostring(L, 2);
        // return field value
        lua_pushstring(L, pTask->getField(szFieldName).c_str());
        return 1;
    }
}
//-----------------------------------------------------------------------
int setLuaTaskField_lua(lua_State* L)
{
    // get task pointer
    intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 1, Meta_Name_LuaTask);
    u2::LuaTask* pTask = (u2::LuaTask*)(*pPointer);
    if (pTask == nullptr)
    {
        assert(0);
        return 0;
    }
    else
    {
        // get field name
        std::string szFieldName = lua_tostring(L, 2);
        // get field value
        std::string szFieldValue = lua_tostring(L, 3);
        // return field value
        pTask->setField(szFieldName, szFieldValue);
        return 0;
    }
}