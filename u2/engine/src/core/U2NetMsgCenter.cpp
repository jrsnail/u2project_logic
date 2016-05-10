//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2NetMsgCenter.h"

#include "U2NetMsg.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> MsgCenterManager* Singleton<MsgCenterManager>::msSingleton = 0;
MsgCenterManager* MsgCenterManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new MsgCenterManager;
	}
	return msSingleton;
}
MsgCenterManager& MsgCenterManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
MsgCenterManager::MsgCenterManager()
{
}
//-----------------------------------------------------------------------
MsgCenterManager::~MsgCenterManager()
{
}
//-----------------------------------------------------------------------
void MsgCenterManager::pushOutMsg(const String& centerName, OutMsg* msg)
{
	MsgCenter* pCenter = retrieveObjectByName(centerName);
	if (pCenter)
	{
		pCenter->pushOutMsg(msg);
	}
}
//-----------------------------------------------------------------------
void MsgCenterManager::pushInMsg(const String& centerName, Msg* msg)
{
	MsgCenter* pCenter = retrieveObjectByName(centerName);
	if (pCenter)
	{
		pCenter->pushInMsg(msg);
	}
}
