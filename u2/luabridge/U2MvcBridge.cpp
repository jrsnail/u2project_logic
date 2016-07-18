//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2MvcBridge.h"

#include "U2Core.h"
#include "U2Mvc.h"
#include "U2GameBase.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
int createContext_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc != 6)
    {
        assert(0);
    }

    std::string szType = lua_tostring(L, 1);
    std::string szName = lua_tostring(L, 2);
    std::string szFacadeName = lua_tostring(L, 3);
    std::string szViewCompType = lua_tostring(L, 4);
    std::string szViewCompName = lua_tostring(L, 5);
    std::string szScriptName = lua_tostring(L, 6);

    // create a pointer as lua userdata
    intptr_t* pPointer = (intptr_t*)lua_newuserdata(L, sizeof(intptr_t));
    // point to u2::Context address
    *pPointer = (intptr_t)u2::ContextManager::getSingletonPtr()->createObject(
        szType, szName, szFacadeName
        , szViewCompType, szViewCompName, szScriptName
        );

    luaL_getmetatable(L, Meta_Name_LuaContext);
    lua_setmetatable(L, -2);

    return 1;
}
//-----------------------------------------------------------------------
int createContextChild_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc != 7)
    {
        assert(0);
    }

    intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 1, Meta_Name_LuaContext);
    u2::Context* pParent = (u2::Context*)(*pPointer);

    std::string szType = lua_tostring(L, 2);
    std::string szName = lua_tostring(L, 3);
    std::string szFacadeName = lua_tostring(L, 4);
    std::string szViewCompType = lua_tostring(L, 5);
    std::string szViewCompName = lua_tostring(L, 6);
    std::string szScriptName = lua_tostring(L, 7);

    if (pParent)
    {
        // create a pointer as lua userdata
        intptr_t* pPointer = (intptr_t*)lua_newuserdata(L, sizeof(intptr_t));
        // point to u2::Context address
        *pPointer = (intptr_t)pParent->createChild(
            szType, szName, szFacadeName
            , szViewCompType, szViewCompName, szScriptName
            );

        luaL_getmetatable(L, Meta_Name_LuaContext);
        lua_setmetatable(L, -2);
    }

    return 1;
}
//-----------------------------------------------------------------------
static void routeContext(u2::Context* context)
{
    if (context == nullptr)
    {
        return;
    }

    // create view component which binding to this context
    u2::ViewComponent* pViewComp = u2::ViewComponentManager::getSingletonPtr()->retrieveObjectByName(context->getViewCompName());
    if (!pViewComp)
    {
        pViewComp = u2::ViewComponentManager::getSingletonPtr()->createObject(context->getViewCompClass(), context->getViewCompName());
    }

    // create lua script 
    u2::Script* pLuaScript = u2::ScriptManager::getSingletonPtr()->createObject(GET_OBJECT_TYPE(u2::CocosLuaScript), BLANK);
    if (pLuaScript)
    {
        pLuaScript->runFile(context->getScriptName());
        u2::Facade* pFacade = u2::FacadeManager::getSingletonPtr()->retrieveObjectByName(context->getFacadeName());
        if (pFacade != nullptr)
        {
            pFacade->registerScript(pLuaScript);
        }
    }

    // traverse children
    u2::Context::ContextMapIterator it = context->getChildIterator();
    while (it.hasMoreElements())
    {
        routeContext(it.getNext());
    }
}
//-----------------------------------------------------------------------
int pushContextQueueBack_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc < 2 || argc > 3)
    {
        assert(0);
    }

    // context queue name
    std::string szQueueName = lua_tostring(L, 1);

    // context user data
    intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 2, Meta_Name_LuaContext);
    u2::Context* pContext = (u2::Context*)(*pPointer);

    // trans type
    u2::ContextQueue::eTransType eType = ContextQueue::eTransType::TT_None;
    if (argc == 3)
    {
        eType = (u2::ContextQueue::eTransType)lua_tointeger(L, 3);
    }

    do
    {
        if (pContext == nullptr)
        {
            break;
        }

        routeContext(pContext);

        u2::Facade* pFacade = u2::FacadeManager::getSingletonPtr()->retrieveObjectByName(ON_Facade_Predefined);
        if (pFacade == nullptr)
        {
            break;
        }
        ContextProxy* contextProxy = pFacade->retrieveProxy<u2::ContextProxy>(ON_Proxy_Context);
        if (contextProxy == nullptr)
        {
            break;
        }
        contextProxy->pushBack(szQueueName, pContext, eType);
    } while (0);
    
    return 0;
}
//-----------------------------------------------------------------------
int pushContextQueueFront_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc < 2 || argc > 3)
    {
        assert(0);
    }

    // context queue name
    std::string szQueueName = lua_tostring(L, 1);

    // context user data
    intptr_t* pPointer = (intptr_t*)luaL_checkudata(L, 2, Meta_Name_LuaContext);
    u2::Context* pContext = (u2::Context*)(*pPointer);

    // trans type
    u2::ContextQueue::eTransType eType = ContextQueue::eTransType::TT_None;
    if (argc == 3)
    {
        eType = (u2::ContextQueue::eTransType)lua_tointeger(L, 3);
    }

    do
    {
        if (pContext == nullptr)
        {
            break;
        }

        routeContext(pContext);

        u2::Facade* pFacade = u2::FacadeManager::getSingletonPtr()->retrieveObjectByName(ON_Facade_Predefined);
        if (pFacade == nullptr)
        {
            break;
        }
        ContextProxy* contextProxy = pFacade->retrieveProxy<u2::ContextProxy>(ON_Proxy_Context);
        if (contextProxy == nullptr)
        {
            break;
        }
        contextProxy->pushFront(szQueueName, pContext, eType);
    } while (0);

    return 0;
}
//-----------------------------------------------------------------------
int retrieveContext_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc != 1)
    {
        assert(0);
    }

    // context name
    std::string szContextName = lua_tostring(L, 1);

    u2::Context* pContext = u2::ContextManager::getSingletonPtr()->retrieveObjectByName(szContextName);
    if (pContext == nullptr)
    {
        lua_pushnil(L);
        return 1;
    }
    else
    {
        // create a pointer as lua userdata
        intptr_t* pPointer = (intptr_t*)lua_newuserdata(L, sizeof(intptr_t));
        // point to u2::Context address
        *pPointer = (intptr_t)pContext;

        luaL_getmetatable(L, Meta_Name_LuaContext);
        lua_setmetatable(L, -2);

        return 1;
    }
}
//-----------------------------------------------------------------------
int registerViewCompEventListenerWithFixedPri_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc != 3)
    {
        assert(0);
    }

    // view component name
    std::string szViewCompName = lua_tostring(L, 1);
    int nEventListenerType = lua_tointeger(L, 2);
    int nPri = lua_tointeger(L, 3);

    CocosViewComponent* pViewComp 
        = (CocosViewComponent*)(u2::ViewComponentManager::getSingletonPtr()->retrieveObjectByName(szViewCompName));
    if (pViewComp == nullptr)
    {
        assert(0);
    }
    else
    {
        pViewComp->registerEventListenerWithFixedPri((cocos2d::EventListener::Type)nEventListenerType, nPri);
    }

    return 0;
}
//-----------------------------------------------------------------------
int registerViewCompEventListenerWithSceneGraphPri_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        assert(0);
    }

    // view component name
    std::string szViewCompName = lua_tostring(L, 1);
    int nEventListenerType = lua_tointeger(L, 2);

    CocosViewComponent* pViewComp
        = (CocosViewComponent*)(u2::ViewComponentManager::getSingletonPtr()->retrieveObjectByName(szViewCompName));
    if (pViewComp == nullptr)
    {
        assert(0);
    }
    else
    {
        pViewComp->registerEventListenerWithSceneGraphPri((cocos2d::EventListener::Type)nEventListenerType);
    }

    return 0;
}
//-----------------------------------------------------------------------
int log_lua(lua_State* L)
{
    // check arg count
    int argc = lua_gettop(L);
    if (argc != 2)
    {
        assert(0);
    }

    int nLevel = lua_tointeger(L, 1);
    std::string szMsg = lua_tostring(L, 2);
    
    u2::LogManager::getSingletonPtr()->logMessage((LogMessageLevel)nLevel, szMsg);
    return 0;
}