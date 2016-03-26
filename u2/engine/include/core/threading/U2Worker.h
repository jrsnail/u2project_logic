#ifndef __U2Worker_H__
#define __U2Worker_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MemoryAllocatorConfig.h"
#include "U2Singleton.h"
#include "U2ThreadHeaders.h"


U2EG_NAMESPACE_BEGIN


class Worker
{
public:
	Worker()
	{
		m_OriginThreadId = getThreadId();
	}

	Worker& operator=(std::thread&& t)
	{
		if (isInitialized())
		{
			assert(0);
		}
		else
		{
			m_Thread = std::move(t);
		}
		return *this; 
	}

	Worker(const Worker& rhs) = delete;
	Worker& operator=(const Worker& rhs) = delete;
	Worker(Worker&& rhs) = delete;
	Worker& operator=(Worker&& rhs) = delete;

	bool isInitialized()
	{
		return !(m_OriginThreadId == getThreadId());
	}

	void join();

	std::thread::id getThreadId() const
	{
		return m_Thread.get_id();
	}

	static String threadId2String(std::thread::id& tid);

protected:
	std::thread			m_Thread;
	std::thread::id		m_OriginThreadId;
};



class WorkerManager : public Singleton < WorkerManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	WorkerManager();

public:
	/** Default destructor.
	*/
	virtual ~WorkerManager();

	template<class _Fn, class... _Args>
	Worker& createObject(_Fn&& _Fx, _Args&&... _Ax)
	{
// 		Worker worker;
// 		worker = std::move(std::thread(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...));
// 		m_Workers[worker.getThreadId()] = worker;
		return worker;
	}

public:
	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static WorkerManager& getSingleton(void);

	/** Override standard Singleton retrieval.
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static WorkerManager* getSingletonPtr(void);

protected:
	typedef map<std::thread::id, Worker>::type				WorkerMap;
	WorkerMap		m_Workers;
};


U2EG_NAMESPACE_END


#endif
