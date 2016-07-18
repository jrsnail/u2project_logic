#ifndef __U2LuaScriptBridge_H__
#define __U2LuaScriptBridge_H__


extern "C"
{

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


int registerLuaScriptWithInterestedNotification_lua(lua_State* L);

int frontC2LTask_lua(lua_State* L);
int destroyC2LTask_lua(lua_State* L);

int createL2CTask_lua(lua_State* L);
int pushL2CTask_lua(lua_State* L);

int getLuaTaskType_lua(lua_State* L);
int getLuaTaskName_lua(lua_State* L);
int getLuaTaskField_lua(lua_State* L);
int setLuaTaskField_lua(lua_State* L);



// table of function names and entry points that will be registered with Lua
static const luaL_reg luaTaskLib[] =
{
    { "getLuaTaskType", getLuaTaskType_lua }, 
    { "getLuaTaskName", getLuaTaskName_lua },
    { "getLuaTaskField", getLuaTaskField_lua },
    { "setLuaTaskField", setLuaTaskField_lua },
    { NULL, NULL }
};

static const char* Meta_Name_LuaTask = "LuaTask";

static int luaopen_meta_LuaTask(lua_State *L)
{
    luaL_newmetatable(L, Meta_Name_LuaTask);
    luaL_register(L, Meta_Name_LuaTask, luaTaskLib);
    return 1;
}


}   // extern "C"


#endif