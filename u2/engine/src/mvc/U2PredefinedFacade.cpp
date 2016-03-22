//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2PredefinedFacade.h"

#include "U2PredefinedCommands.h"
#include "U2PredefinedProxies.h"
#include "U2PredefinedPrerequisites.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
PredefinedFacade* PredefinedFacade::ms_pSingleton = nullptr;
//-----------------------------------------------------------------------
PredefinedFacade::PredefinedFacade(const String& type, const String& name)
    : Facade(type, name)
{
	if (ms_pSingleton != nullptr)
	{
		assert(0);
		return;
	}

	ms_pSingleton = this;

    // command factory
    CREATE_FACTORY(DestoryContextCommand);
    CREATE_FACTORY(BackKeyCommand);
    CREATE_FACTORY(TransCommand);

    // proxy factory
    CREATE_FACTORY(ContextProxy);

    // mediator factory
    CREATE_FACTORY(TransMediator);

    // viewcomponent factory
}
//-----------------------------------------------------------------------
PredefinedFacade::~PredefinedFacade(void)
{
	ms_pSingleton = nullptr;
}
//-----------------------------------------------------------------------
void PredefinedFacade::initializeController(void)
{
    Facade::initializeController();

    registerCommand(NTF_Predefined_DestroyContext, CommandManager::getSingleton().createObject(OT_DestoryContextCommand, BLANK));
    registerCommand(NTF_Predefined_BackKey, CommandManager::getSingleton().createObject(OT_BackKeyCommand, BLANK));
    registerCommand(NTF_Predefined_Trans, CommandManager::getSingleton().createObject(OT_TransCommand, BLANK));
}
//-----------------------------------------------------------------------
void PredefinedFacade::initializeModel(void)
{
	Facade::initializeModel();

    registerProxy(ProxyManager::getSingleton().createObject(OT_ContextProxy, ON_Proxy_Context));
}
//-----------------------------------------------------------------------
void PredefinedFacade::initializeView(void)
{
	Facade::initializeView();
}