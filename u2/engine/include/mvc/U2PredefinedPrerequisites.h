//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2PredefinedPrerequisites__
#define __U2PredefinedPrerequisites__


#include "U2Core.h"



U2EG_NAMESPACE_USING



extern const char* OT_Facade;
extern const char* OT_Model;
extern const char* OT_View;
extern const char* OT_Observer;
extern const char* OT_Controller;
extern const char* OT_Notification;
extern const char* OT_Context;

// facade
extern const char* OT_PredefinedFacade;

// proxy
extern const char* OT_ContextProxy;

// command
extern const char* OT_DestoryContextCommand;
extern const char* OT_BackKeyCommand;
extern const char* OT_TransCommand;

// mediator
extern const char* OT_TransMediator;

// view

// context queue
extern const char* OT_SingleContextQueue;
extern const char* OT_InfiniteContextQueue;

// notification
static const char* NTF_Predefined_DestroyContext = "NTF_Predefined_DestroyContext";
static const char* NTF_Predefined_BackKey = "NTF_Predefined_BackKey";
static const char* NTF_Predefined_Trans = "NTF_Predefined_Trans";

// facade name
static const char* ON_Facade_Predefined = "ON_Facade_Predefined";

// proxy name
static const char* ON_Proxy_Context = "ON_Proxy_Context";


// data pool
static const char* ON_DataPool_Task = "ON_DataPool_Task";

// task loop
static const char* ON_Logic_TaskLoop = "ON_Logic_TaskLoop";



#endif /* defined(__PredefinedPrerequisites__) */
