#ifndef __U2AllLuaBridge_H__
#define __U2AllLuaBridge_H__


#include "U2LuaScriptBridge.h"
#include "U2MvcBridge.h"
#include "U2DataModelBridge.h"
#include "U2ConstantBridge.h"
#include "U2GameBridge.h"


extern "C"
{


#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"




    // table of function names and entry points that will be registered with Lua
    static const luaL_reg u2lib[] =
    {
        // log
        { "log", log_lua },
        // LuaScript
        //{ "createLuaScript", createLuaScript_lua },
        { "registerLuaScriptWithInterestedNotification", registerLuaScriptWithInterestedNotification_lua },
        { "frontC2LTask", frontC2LTask_lua },
        { "destroyC2LTask", destroyC2LTask_lua },
        { "createL2CTask", createL2CTask_lua },
        { "pushL2CTask", pushL2CTask_lua },
        { "getLuaTaskType", getLuaTaskType_lua },
        { "getLuaTaskName", getLuaTaskName_lua },
        { "getLuaTaskField", getLuaTaskField_lua },
        { "setLuaTaskField", setLuaTaskField_lua },
        // Context
        { "createContext", createContext_lua },
        { "createContextChild", createContextChild_lua },
        { "pushContextQueueBack", pushContextQueueBack_lua },
        { "pushContextQueueFront", pushContextQueueFront_lua },
        { "retrieveContext", retrieveContext_lua },
        // view component
        { "registerViewCompEventListenerWithFixedPri", registerViewCompEventListenerWithFixedPri_lua },
        { "registerViewCompEventListenerWithSceneGraphPri", registerViewCompEventListenerWithSceneGraphPri_lua },
        // game

        { NULL, NULL }
    };

    // register u2 as a module
    static int luaopen_u2lib(lua_State* L)
    {
        luaL_register(L, "u2", u2lib);
        return 1;
    }



}   // extern "C"


#endif