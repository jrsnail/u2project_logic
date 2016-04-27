#include "U2NetMsg.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> MsgManager* Singleton<MsgManager>::msSingleton = 0;
MsgManager* MsgManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new MsgManager;
	}
	return msSingleton;
}
MsgManager& MsgManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
MsgManager::MsgManager()
{
}
//-----------------------------------------------------------------------
MsgManager::~MsgManager()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Protocol::Protocol(const String& type)
	: ReusableObject(type)
{

}
//-----------------------------------------------------------------------
Protocol::~Protocol()
{

}
//-----------------------------------------------------------------------
void initialize(const String& outType, const String& inType);
//-----------------------------------------------------------------------
String Protocol::serialize()
{
	return BLANK;
}
//-----------------------------------------------------------------------
void Protocol::deserialize(const String& str)
{

}
//-----------------------------------------------------------------------
void Protocol::preRecycleByPool()
{
	if (m_pOutMsg != nullptr)
	{
		MsgManager::getSingleton().recycleObject(m_pOutMsg);
		m_pOutMsg = nullptr;
	}
	if (m_pInMsg != nullptr)
	{
		MsgManager::getSingleton().recycleObject(m_pInMsg);
		m_pInMsg = nullptr;
	}
	if (m_pPredictedMsg != nullptr)
	{
		MsgManager::getSingleton().recycleObject(m_pPredictedMsg);
		m_pPredictedMsg = nullptr;
	}

	ReusableObject::preRecycleByPool();
}
//-----------------------------------------------------------------------
void Protocol::postReuseFromPool()
{
	ReusableObject::postReuseFromPool();

	m_pOutMsg = nullptr;
	m_pInMsg = nullptr;
	m_pPredictedMsg = nullptr;
}
//-----------------------------------------------------------------------
Msg& Protocol::getOutMsg() const
{
	assert(m_pOutMsg != nullptr);
	return *m_pOutMsg;
}
//-----------------------------------------------------------------------
Msg& Protocol::getInMsg() const
{
	assert(m_pInMsg != nullptr);
	return *m_pInMsg;
}
//-----------------------------------------------------------------------
Msg& Protocol::getPredictedMsg() const
{
	assert(m_pPredictedMsg != nullptr);
	return *m_pPredictedMsg;
}
//-----------------------------------------------------------------------
void Protocol::setOutMsg(Msg* msg)
{
	assert(m_pOutMsg == nullptr);
	assert(msg != nullptr);
	m_pOutMsg = msg;
}
//-----------------------------------------------------------------------
void Protocol::setInMsg(Msg* msg)
{
	assert(m_pInMsg == nullptr);
	assert(msg != nullptr);
	assert(m_pOutMsg != nullptr);
	assert(m_pOutMsg->getProtocolName() == msg->getProtocolName());
	m_pInMsg = msg;
}
//-----------------------------------------------------------------------
void Protocol::setPredictedMsg(Msg* msg)
{
	assert(m_pPredictedMsg == nullptr);
	assert(msg != nullptr);
	assert(m_pOutMsg != nullptr);
	assert(m_pOutMsg->getProtocolName() == msg->getProtocolName());
	m_pPredictedMsg = msg;
}