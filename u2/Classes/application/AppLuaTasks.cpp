#include "AppLuaTasks.h"

#include "AppPrerequisites.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CreateLuaScriptLuaTask::CreateLuaScriptLuaTask(const std::string& type, const std::string& name)
: LuaTask(type, name)
{
}
//-----------------------------------------------------------------------
CreateLuaScriptLuaTask::~CreateLuaScriptLuaTask()
{
}
//-----------------------------------------------------------------------
void CreateLuaScriptLuaTask::run()
{
    std::string szLuaFileName = getField("LuaFileName");
    std::string szFacadeName = getField("FacadeName");

    u2::Script* pLuaScript = u2::ScriptManager::getSingletonPtr()->createObject(GET_OBJECT_TYPE(u2::CocosLuaScript), BLANK);
    if (pLuaScript)
    {
        pLuaScript->runFile(szLuaFileName);
        u2::Facade* pFacade = u2::FacadeManager::getSingletonPtr()->retrieveObjectByName(szFacadeName);
        if (pFacade != nullptr)
        {
            pFacade->registerScript(pLuaScript);
        }
    }
    else
    {
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SetViewCompUiNameLuaTask::SetViewCompUiNameLuaTask(const std::string& type, const std::string& name)
    : LuaTask(type, name)
{
}
//-----------------------------------------------------------------------
SetViewCompUiNameLuaTask::~SetViewCompUiNameLuaTask()
{
}
//-----------------------------------------------------------------------
void SetViewCompUiNameLuaTask::run()
{
    std::string szViewCompName = getField("ViewCompName");
    std::string szUiName = getField("UiName");

    u2::ViewComponent* pViewComp = u2::ViewComponentManager::getSingletonPtr()->retrieveObjectByName(szViewCompName);
    if (pViewComp == nullptr)
    {
        assert(0);
    }
    else
    {
        pViewComp->initialize(szUiName);
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SetViewCompEnterActionLuaTask::SetViewCompEnterActionLuaTask(const std::string& type, const std::string& name)
    : LuaTask(type, name)
{
}
//-----------------------------------------------------------------------
SetViewCompEnterActionLuaTask::~SetViewCompEnterActionLuaTask()
{
}
//-----------------------------------------------------------------------
void SetViewCompEnterActionLuaTask::run()
{
    std::string szViewCompName = getField("ViewCompName");
    CocosViewComponent::eTransAction eTrans
        = (CocosViewComponent::eTransAction)u2::StringUtil::parseInt(getField("EnterAction"));

    CocosViewComponent* pViewComp
        = dynamic_cast<CocosViewComponent*>(u2::ViewComponentManager::getSingletonPtr()->retrieveObjectByName(szViewCompName));
    if (pViewComp == nullptr)
    {
        assert(0);
    }
    else
    {
        pViewComp->setEnterTransAction(eTrans);
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
SetViewCompExitActionLuaTask::SetViewCompExitActionLuaTask(const std::string& type, const std::string& name)
    : LuaTask(type, name)
{
}
//-----------------------------------------------------------------------
SetViewCompExitActionLuaTask::~SetViewCompExitActionLuaTask()
{
}
//-----------------------------------------------------------------------
void SetViewCompExitActionLuaTask::run()
{
    std::string szViewCompName = getField("ViewCompName");
    CocosViewComponent::eTransAction eTrans
        = (CocosViewComponent::eTransAction)u2::StringUtil::parseInt(getField("ExitAction"));

    CocosViewComponent* pViewComp
        = dynamic_cast<CocosViewComponent*>(u2::ViewComponentManager::getSingletonPtr()->retrieveObjectByName(szViewCompName));
    if (pViewComp == nullptr)
    {
        assert(0);
    }
    else
    {
        pViewComp->setExitTransAction(eTrans);
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CenterViewCompLuaTask::CenterViewCompLuaTask(const std::string& type, const std::string& name)
    : LuaTask(type, name)
{
}
//-----------------------------------------------------------------------
CenterViewCompLuaTask::~CenterViewCompLuaTask()
{
}
//-----------------------------------------------------------------------
void CenterViewCompLuaTask::run()
{
    std::string szViewCompName = getField("ViewCompName");

    CocosViewComponent* pViewComp
        = dynamic_cast<CocosViewComponent*>(u2::ViewComponentManager::getSingletonPtr()->retrieveObjectByName(szViewCompName));
    if (pViewComp == nullptr)
    {
        assert(0);
    }
    else
    {
        pViewComp->centerRootNode();
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PreloadLuaTask::PreloadLuaTask(const std::string& type, const std::string& name)
: LuaTask(type, name)
{
}
//-----------------------------------------------------------------------
PreloadLuaTask::~PreloadLuaTask()
{
}
//-----------------------------------------------------------------------
void PreloadLuaTask::run()
{
    std::thread t = std::thread(&PreloadLuaTask::_preload, this);
    t.detach();
}
//-----------------------------------------------------------------------
void PreloadLuaTask::_preload()
{
    // todo: preload

    // push preload end task
    u2::Script* pScript = u2::LuaScriptManager::getSingletonPtr()->retrieveObjectByName(m_szScriptName);
    u2::LuaScript* pLuaScript = dynamic_cast<u2::LuaScript*>(pScript);
    if (pLuaScript != nullptr)
    {
        u2::LuaTask* pC2LTask = pLuaScript->createLuaTask(OT_C2LTask_PreloadEnd, BLANK);
        pLuaScript->pushC2LTask(pC2LTask);
    }
}