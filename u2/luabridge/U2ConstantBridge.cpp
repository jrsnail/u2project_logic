#include "U2ConstantBridge.h"

#include "U2GameBase.h"
#include "application/AppPrerequisites.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
static void setNumberfield(lua_State* L, const char* key, float value)
{
    lua_pushnumber(L, value);
    lua_setfield(L, -2, key);
}
//-----------------------------------------------------------------------
static void setIntegerfield(lua_State* L, const char* key, int value)
{
    lua_pushinteger(L, value);
    lua_setfield(L, -2, key);
}
//-----------------------------------------------------------------------
static void setStringfield(lua_State* L, const char* key, const char* value)
{
    lua_pushstring(L, value);
    lua_setfield(L, -2, key);
}
//-----------------------------------------------------------------------
void createConstant4Lua(lua_State* L, u2::Script* script)
{
    lua_newtable(L);

    // script constant value
    setStringfield(L, "ScriptName", script->getName().c_str());

    // event listener
    setIntegerfield(L, "EventListenerType_TouchOneByOne", (int)cocos2d::EventListener::Type::TOUCH_ONE_BY_ONE);
    setIntegerfield(L, "EventListenerType_TouchAllAtOnce", (int)cocos2d::EventListener::Type::TOUCH_ALL_AT_ONCE);
    setIntegerfield(L, "EventListenerType_Keyboard", (int)cocos2d::EventListener::Type::KEYBOARD);

    // object type
    setStringfield(L, "OT_Context", OT_Context);
    setStringfield(L, "OT_CocosViewComponent", OT_CocosViewComponent);
    setStringfield(L, "ON_Facade_App", ON_Facade_App);
    setStringfield(L, "OT_CreateLuaScriptLuaTask", OT_CreateLuaScriptLuaTask);
    setStringfield(L, "OT_SetViewCompUiNameLuaTask", OT_SetViewCompUiNameLuaTask);
    setStringfield(L, "OT_SetViewCompEnterActinLuaTask", OT_SetViewCompEnterActinLuaTask);
    setStringfield(L, "OT_SetViewCompExitActinLuaTask", OT_SetViewCompExitActinLuaTask);
    setStringfield(L, "OT_CenterViewCompLuaTask", OT_CenterViewCompLuaTask); 
    setStringfield(L, "OT_PreloadLuaTask", OT_PreloadLuaTask);

    // context queue name
    setStringfield(L, "ON_ContextQueue_Shade", ON_ContextQueue_Shade);
    setStringfield(L, "ON_ContextQueue_Scene", ON_ContextQueue_Scene);
    setStringfield(L, "ON_ContextQueue_Tab", ON_ContextQueue_Tab);
    setStringfield(L, "ON_ContextQueue_Dialog", ON_ContextQueue_Dialog);
    setStringfield(L, "ON_ContextQueue_Common", ON_ContextQueue_Common);

    // context trans type
    setIntegerfield(L, "TransType_TT_None", (int)u2::ContextQueue::eTransType::TT_None);
    setIntegerfield(L, "TransType_TT_Overlay", (int)u2::ContextQueue::eTransType::TT_Overlay);
    setIntegerfield(L, "TransType_TT_OneByOne", (int)u2::ContextQueue::eTransType::TT_OneByOne);
    setIntegerfield(L, "TransType_TT_Cross", (int)u2::ContextQueue::eTransType::TT_Cross);

    // log
    setIntegerfield(L, "LML_Trivial", (int)u2::LogMessageLevel::LML_TRIVIAL);
    setIntegerfield(L, "LML_Normal", (int)u2::LogMessageLevel::LML_NORMAL);
    setIntegerfield(L, "LML_Critical", (int)u2::LogMessageLevel::LML_CRITICAL);

    // lua 2 c task
    setStringfield(L, "OT_L2CTask_CreateLuaScript", OT_L2CTask_CreateLuaScript);

    lua_setglobal(L, "Constant");
}