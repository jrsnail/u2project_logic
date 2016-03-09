#include "U2Worker.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Worker::Worker(const String& type, const String& name)
	: Object(type, name)
{
}
//-----------------------------------------------------------------------
void Worker::join() 
{ 
	if (m_Thread.joinable())
		m_Thread.join();
}
//-----------------------------------------------------------------------
std::thread::id Worker::getThreadId() const
{
	return m_Thread.get_id();
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
	CREATE_FACTORY(Worker);
}
//-----------------------------------------------------------------------
WorkerManager::~WorkerManager()
{
}
//-----------------------------------------------------------------------
Worker* WorkerManager::createObject(const String& type, const String& name)
{
	return SimpleObjectManager<Worker>::createObject(type, name);
}