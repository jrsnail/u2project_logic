#include "U2Worker.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void Worker::join() 
{ 
	if (m_Thread.joinable())
		m_Thread.join();
}
//-----------------------------------------------------------------------
String Worker::threadId2String(std::thread::id& tid)
{
	StringStream stream;
	tid._To_text(stream);
	return stream.str();
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> WorkerManager* Singleton<WorkerManager>::msSingleton = 0;
WorkerManager* WorkerManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new WorkerManager;
	}
	return msSingleton;
}
WorkerManager& WorkerManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
WorkerManager::WorkerManager()
{
}
//-----------------------------------------------------------------------
WorkerManager::~WorkerManager()
{
	m_Workers.clear();
}