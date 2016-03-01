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
template<> PredefinedFacade* Singleton<PredefinedFacade>::msSingleton = 0;
//-----------------------------------------------------------------------
PredefinedFacade* PredefinedFacade::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
        Facade* pFacade = FacadeManager::getSingleton().retrieveObject(ON_Facade_Predefined);
		if (pFacade == nullptr)
		{
            pFacade = Facade::createFacade<PredefinedFacade>(ON_Facade_Predefined);
		}
		msSingleton = dynamic_cast<PredefinedFacade*>(pFacade);
	}
	
	return msSingleton;
}
//-----------------------------------------------------------------------
PredefinedFacade& PredefinedFacade::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
PredefinedFacade::PredefinedFacade(const String& type, const String& name)
    : Facade(type, name)
{
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