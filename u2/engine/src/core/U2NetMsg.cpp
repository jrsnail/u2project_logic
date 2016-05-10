#include "U2NetMsg.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Msg::Msg(const String& type)
	: ReusableObject(type)
{
}
//-----------------------------------------------------------------------
Msg::~Msg()
{
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
OutMsg::OutMsg(const String& type)
	: Msg(type)
	, m_eMsgType(MT_SYNCHRONOUS)
{
}
//-----------------------------------------------------------------------
OutMsg::~OutMsg()
{
}
//-----------------------------------------------------------------------
void OutMsg::initialize(const String& outMsgName)
{
	assert(outMsgName == m_szName);
	m_szOutMsgName = m_szName;
}
//-----------------------------------------------------------------------
const u2char* const OutMsg::getData()
{
	if (m_Data.size() != 0)
		return &(m_Data.front());

	return nullptr;
}
//-----------------------------------------------------------------------
size_t OutMsg::getDataSize()
{
	return m_Data.size();
}
//-----------------------------------------------------------------------
OutMsg::MsgType OutMsg::getMsgType() const
{
	return m_eMsgType;
}
//-----------------------------------------------------------------------
void OutMsg::setInMsg(InMsg* msg)
{
	assert(m_pInMsg == nullptr);
	assert(msg != nullptr);
	assert(m_szName == msg->getOutMsgName());
	m_pInMsg = msg;
}
//-----------------------------------------------------------------------
void OutMsg::setPredictedMsg(InMsg* msg)
{
	assert(m_pPredictedMsg == nullptr);
	assert(msg != nullptr);
	assert(m_szName == msg->getOutMsgName());
	m_pPredictedMsg = msg;
}
//-----------------------------------------------------------------------
InMsg* OutMsg::getInMsg() const
{
	assert(m_pInMsg != nullptr);
	return m_pInMsg;
}
//-----------------------------------------------------------------------
InMsg* OutMsg::getPredictedMsg() const
{
	assert(m_pPredictedMsg != nullptr);
	return m_pPredictedMsg;
}
//-----------------------------------------------------------------------
void OutMsg::preRecycleByPool()
{
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
	m_Data.clear();
	m_eMsgType = MT_SYNCHRONOUS;

	ReusableObject::preRecycleByPool();
}
//-----------------------------------------------------------------------
void OutMsg::postReuseFromPool()
{
	ReusableObject::postReuseFromPool();

	m_pInMsg = nullptr;
	m_pPredictedMsg = nullptr;
	m_Data.clear();
	m_eMsgType = MT_SYNCHRONOUS;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
InMsg::InMsg(const String& type)
	: Msg(type)
{
}
//-----------------------------------------------------------------------
InMsg::~InMsg()
{
}
//-----------------------------------------------------------------------
u2char* InMsg::getData()
{
	if (m_Data.size() != 0)
		return &(m_Data.front());

	return nullptr;
}
//-----------------------------------------------------------------------
size_t InMsg::getDataSize()
{
	return m_Data.size();
}
//-----------------------------------------------------------------------
void InMsg::preRecycleByPool()
{
	m_Data.clear();

	Msg::preRecycleByPool();
}
//-----------------------------------------------------------------------
void InMsg::postReuseFromPool()
{
	Msg::postReuseFromPool();

	m_Data.clear();
}
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