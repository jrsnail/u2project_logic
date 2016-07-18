//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "AppCommands.h"

#include "AppPrerequisites.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
StartupCommand::StartupCommand(const std::string& type, const std::string& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
StartupCommand::~StartupCommand()
{
}
//-----------------------------------------------------------------------
void StartupCommand::go(const Notification& notification)
{
    Notification ntf(NTF_App_Trans2Shade);
    getFacade().broadcastNotification(ntf);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Trans2ShadeCommand::Trans2ShadeCommand(const std::string& type, const std::string& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
Trans2ShadeCommand::~Trans2ShadeCommand()
{
}
//-----------------------------------------------------------------------
void Trans2ShadeCommand::go(const Notification& notification)
{
    // create context tree
    u2::Context* pRoot = ContextManager::getSingletonPtr()->createObject(
        OT_Context, ON_Context_Root
        , getFacade().getName()
        , OT_ShadeViewComponent, "ShadeViewComponent"
        , BLANK);
    ContextProxy* pContextProxy = getFacade(ON_Facade_Predefined).retrieveProxy<ContextProxy>(ON_Proxy_Context);
    pContextProxy->pushBack(ON_ContextQueue_Shade, pRoot);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
StartupLuaCommand::StartupLuaCommand(const std::string& type, const std::string& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
StartupLuaCommand::~StartupLuaCommand()
{
}
//-----------------------------------------------------------------------
void StartupLuaCommand::go(const Notification& notification)
{
    // main lua_State
    u2::Script* pScript = u2::ScriptManager::getSingletonPtr()->createObject(GET_OBJECT_TYPE(CocosLuaScript), "luastate_main");
    if (pScript != nullptr)
    {
        pScript->runFile("main.lua");
        getFacade().registerScript(pScript);
    }

//     // logo view component
//     u2::Script* pScript = u2::ScriptManager::getSingletonPtr()->createObject(GET_OBJECT_TYPE(CocosLuaScript), "luastate_logo");
//     if (pScript != nullptr)
//     {
//         pScript->runFile("LogoViewComp.lua");
//         getFacade().registerScript(pScript);
//     }

    // info view component
//     u2::Script* pScript = u2::ScriptManager::getSingletonPtr()->createObject(GET_OBJECT_TYPE(CocosLuaScript), "luastate_info");
//     if (pScript != nullptr)
//     {
//         pScript->runFile("InfoViewComp.lua");
//         getFacade().registerScript(pScript);
//     }
}
/*
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
HotUpdateCommand::HotUpdateCommand(const std::string& type, const std::string& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
HotUpdateCommand::~HotUpdateCommand()
{
}
//-----------------------------------------------------------------------
void HotUpdateCommand::go(const Notification& notification)
{
    typedef std::tuple<std::string>   HotUpdateCommandData;
    const HotUpdateCommandData* pData = static_cast<const HotUpdateCommandData*>(notification.getData());
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        const std::string& szJson = std::get<0>(*pData);
        _parseServerJson(szJson);
        _parseLocalJson();
        _calUpdateList();
        _startDownload();
        
    }
}
//-----------------------------------------------------------------------
void HotUpdateCommand::_parseServerJson(const std::string& json)
{

}
//-----------------------------------------------------------------------
void HotUpdateCommand::_parseLocalJson()
{
    std::string szFolderName;
    DataPool::getSingletonPtr()->gaveStringData("HotUpdate", "FolderName", szFolderName);
    std::string szJson;
    DataPool::getSingletonPtr()->gaveStringData("HotUpdate", "UpdatingList", szJson);
    // parse json
    m_stUpdatingCollection;

}
//-----------------------------------------------------------------------
void HotUpdateCommand::_calUpdateList()
{
    // traverse from lower version to upper version
    for (VersionMap::const_iterator itVer = m_VersionMap.begin();
        itVer != m_VersionMap.end(); itVer++)
    {
        if (!(m_stUpdatingCollection.version < itVer->first))
        {
            continue;
        }

        const FileList& fileList = itVer->second;
        // traverse each file in this version
        for (FileList::const_iterator it = fileList.begin();
            it != fileList.end(); it++)
        {
            const std::string& szKey = it->first;
            const StUpdatingFile& stFile = it->second;

            FileList::const_iterator itUpdating = m_stUpdatingCollection.updatingList.find(szKey);
            if (itUpdating != m_stUpdatingCollection.updatingList.end())
            {
                // cancel uncompleted update
                if (!itUpdating->second.bCompleted)
                {
                }
            }

            // upper version always over write files which has same key in lower version, 
            // and insert new file to 'will update' list
            m_stUpdatingCollection.updatingList[szKey] = stFile;
        }

        m_stUpdatingCollection.version = itVer->first;
    }
}
//-----------------------------------------------------------------------
void HotUpdateCommand::_startDownload()
{
    // traverse each file in 'will update' list
    for (FileList::const_iterator it = m_stUpdatingCollection.updatingList.begin();
        it != m_stUpdatingCollection.updatingList.end(); it++)
    {
        // add to download queue
        const StUpdatingFile& stUpdatingFile = it->second;
        DataPool::getSingletonPtr()->downloadFile(stUpdatingFile.szUrl
            , stUpdatingFile.szFolder + "/" + stUpdatingFile.szTmpFilename);
    }
}
*/