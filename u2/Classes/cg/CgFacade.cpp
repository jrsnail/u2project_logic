//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "CgFacade.h"

#include "CgCommands.h"
#include "CgMediators.h"
#include "CgViewComponents.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> CgFacade* Singleton<CgFacade>::msSingleton = 0;
//-----------------------------------------------------------------------
CgFacade* CgFacade::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
        Facade* pFacade = FacadeManager::getSingleton().retrieveObjectByName(ON_Facade_Cg);
		if (pFacade == nullptr)
		{
            pFacade = Facade::createFacade<CgFacade>(ON_Facade_Cg);
		}
		msSingleton = dynamic_cast<CgFacade*>(pFacade);
	}
	
	return msSingleton;
}
//-----------------------------------------------------------------------
CgFacade& CgFacade::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
CgFacade::CgFacade(const u2::String& type, const u2::String& name)
    : Facade(type, name)
{
    // command factory
    CREATE_FACTORY(Trans2CgCommand);
    CREATE_FACTORY(Trans2StartPageCommand);

    // mediator factory
    CREATE_FACTORY(CgMediator);
    CREATE_FACTORY(StartPageMediator);

    // viewcomponent factory
    CREATE_FACTORY(CgViewComponent);
    CREATE_FACTORY(StartPageViewComponent);
}
//-----------------------------------------------------------------------
CgFacade::~CgFacade(void)
{
}
//-----------------------------------------------------------------------
void CgFacade::initializeController(void)
{
    Facade::initializeController();

    registerCommand(NTF_Cg_Trans2Cg, CommandManager::getSingleton().createObject(OT_Trans2CgCommand, BLANK));
    registerCommand(NTF_Cg_Trans2StartPage, CommandManager::getSingleton().createObject(OT_Trans2StartPageCommand, BLANK));
}
//-----------------------------------------------------------------------
void CgFacade::initializeModel(void)
{
	Facade::initializeModel();
}
//-----------------------------------------------------------------------
void CgFacade::initializeView(void)
{
	Facade::initializeView();
}