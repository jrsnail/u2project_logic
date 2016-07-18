#ifndef __U2MvcBridge_H__
#define __U2MvcBridge_H__


extern "C"
{

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


int createContext_lua(lua_State* L);
int createContextChild_lua(lua_State* L);

int pushContextQueueBack_lua(lua_State* L);
int pushContextQueueFront_lua(lua_State* L);

int retrieveContext_lua(lua_State* L);

int registerViewCompEventListenerWithFixedPri_lua(lua_State* L);
int registerViewCompEventListenerWithSceneGraphPri_lua(lua_State* L);

int log_lua(lua_State* L);



// table of function names and entry points that will be registered with Lua
static const luaL_reg luaContextlib[] =
{
    { "createContext", createContext_lua },
    { "createContextChild", createContextChild_lua },
    { "pushContextQueueBack", pushContextQueueBack_lua },
    { "pushContextQueueFront", pushContextQueueFront_lua },
    { "retrieveContext", retrieveContext_lua },
    { NULL, NULL }
};

static const char* Meta_Name_LuaContext = "LuaContext";

static int luaopen_meta_Context(lua_State *L)
{
    luaL_newmetatable(L, Meta_Name_LuaContext);
    luaL_register(L, Meta_Name_LuaContext, luaContextlib);
    return 1;
}




}   // extern "C"


#endif