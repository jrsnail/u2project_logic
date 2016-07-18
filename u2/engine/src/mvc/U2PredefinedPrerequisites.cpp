//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#include "U2PredefinedPrerequisites.h"

#include "U2Facade.h"
#include "U2Model.h"
#include "U2View.h"
#include "U2Observer.h"
#include "U2Controller.h"
#include "U2Notification.h"
#include "U2Context.h"
#include "U2PredefinedFacade.h"
#include "U2PredefinedCommands.h"
#include "U2PredefinedMediators.h"
#include "U2PredefinedProxies.h"
#include "U2ContextQueue.h"
#include "U2PredefinedLuaTasks.h"


U2EG_NAMESPACE_USING


const char* OT_Facade = GET_OBJECT_TYPE(Facade);
const char* OT_Model = GET_OBJECT_TYPE(Model);
const char* OT_View = GET_OBJECT_TYPE(View);
const char* OT_Observer = GET_OBJECT_TYPE(Observer);
const char* OT_Controller = GET_OBJECT_TYPE(Controller);
const char* OT_Notification = GET_OBJECT_TYPE(Notification);
const char* OT_Context = GET_OBJECT_TYPE(u2::Context);


// facade
const char* OT_PredefinedFacade = GET_OBJECT_TYPE(PredefinedFacade);

// proxy
const char* OT_ContextProxy = GET_OBJECT_TYPE(ContextProxy);

// command
const char* OT_DestoryContextCommand = GET_OBJECT_TYPE(DestoryContextCommand);
const char* OT_BackKeyCommand = GET_OBJECT_TYPE(BackKeyCommand);
const char* OT_TransCommand = GET_OBJECT_TYPE(TransCommand);

// mediator
const char* OT_TransMediator = GET_OBJECT_TYPE(TransMediator);

// context queue
const char* OT_SingleContextQueue = GET_OBJECT_TYPE(SingleContextQueue);
const char* OT_InfiniteContextQueue = GET_OBJECT_TYPE(InfiniteContextQueue);
