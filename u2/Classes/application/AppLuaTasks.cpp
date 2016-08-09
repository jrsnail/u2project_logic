#include "AppLuaTasks.h"

#include "AppPrerequisites.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CreateLuaScriptLuaTask::CreateLuaScriptLuaTask(const String& type, const String& name, const String& guid)
: LuaTask(type, name, guid)
{
}
//-----------------------------------------------------------------------
CreateLuaScriptLuaTask::~CreateLuaScriptLuaTask()
{
}
//-----------------------------------------------------------------------
void CreateLuaScriptLuaTask::run()
{
    String szLuaFileName = getField("LuaFileName");
    String szFacadeName = getField("FacadeName");

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
SetViewCompUiNameLuaTask::SetViewCompUiNameLuaTask(const String& type, const String& name, const String& guid)
    : LuaTask(type, name, guid)
{
}
//-----------------------------------------------------------------------
SetViewCompUiNameLuaTask::~SetViewCompUiNameLuaTask()
{
}
//-----------------------------------------------------------------------
void SetViewCompUiNameLuaTask::run()
{
    String szViewCompName = getField("ViewCompName");
    String szUiName = getField("UiName");

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
SetViewCompEnterActionLuaTask::SetViewCompEnterActionLuaTask(const String& type, const String& name, const String& guid)
    : LuaTask(type, name, guid)
{
}
//-----------------------------------------------------------------------
SetViewCompEnterActionLuaTask::~SetViewCompEnterActionLuaTask()
{
}
//-----------------------------------------------------------------------
void SetViewCompEnterActionLuaTask::run()
{
    String szViewCompName = getField("ViewCompName");
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
SetViewCompExitActionLuaTask::SetViewCompExitActionLuaTask(const String& type, const String& name, const String& guid)
    : LuaTask(type, name, guid)
{
}
//-----------------------------------------------------------------------
SetViewCompExitActionLuaTask::~SetViewCompExitActionLuaTask()
{
}
//-----------------------------------------------------------------------
void SetViewCompExitActionLuaTask::run()
{
    String szViewCompName = getField("ViewCompName");
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
CenterViewCompLuaTask::CenterViewCompLuaTask(const String& type, const String& name, const String& guid)
    : LuaTask(type, name, guid)
{
}
//-----------------------------------------------------------------------
CenterViewCompLuaTask::~CenterViewCompLuaTask()
{
}
//-----------------------------------------------------------------------
void CenterViewCompLuaTask::run()
{
    String szViewCompName = getField("ViewCompName");

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
PreloadLuaTask::PreloadLuaTask(const String& type, const String& name, const String& guid)
: LuaTask(type, name, guid)
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