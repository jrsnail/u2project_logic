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

private:
    U2_DISALLOW_COPY_AND_ASSIGN(Worker);

protected:
	std::thread			m_Thread;
	std::thread::id		m_OriginThreadId;
};


U2EG_NAMESPACE_END


#endif
