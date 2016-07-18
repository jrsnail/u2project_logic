#ifndef __U2ConstantBridge_H__
#define __U2ConstantBridge_H__


#include "U2GameBase.h"


extern "C"
{


#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


void createConstant4Lua(lua_State* L, u2::Script* script);



}   // extern "C"


#endif