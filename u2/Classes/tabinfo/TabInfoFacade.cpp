//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "TabInfoFacade.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TabInfoFacade::TabInfoFacade(const String& type, const String& name)
    : Facade(type, name)
{
}
//-----------------------------------------------------------------------
TabInfoFacade& TabInfoFacade::getInstance(void)
{
    Facade* pFacade = FacadeManager::getSingleton().retrieveObjectByName("TabInfoFacade");
    if (pFacade == nullptr)
    {
        pFacade = FacadeManager::getSingleton().createObject(ObjectTypeTabInfoFacade, "TabInfoFacade");
    }
    return *(dynamic_cast<TabInfoFacade*>(pFacade));
}
//-----------------------------------------------------------------------
TabInfoFacade::~TabInfoFacade(void)
{
}
//-----------------------------------------------------------------------
void TabInfoFacade::initializeController(void)
{
    Facade::initializeController();
	registerCommand(Ntf_Tabinfo_Create, CommandManager::getSingleton().createObject(ObjectTypeTabInfoCreateCommand, BLANK));
	registerCommand(Ntf_Tabinfo_Destroy, CommandManager::getSingleton().createObject(ObjectTypeTabInfoDestroyCommand, BLANK));
}