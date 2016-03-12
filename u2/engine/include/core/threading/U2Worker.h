#ifndef __U2Worker_H__
#define __U2Worker_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MemoryAllocatorConfig.h"
#include "U2Singleton.h"
#include "U2SimpleObjectManager.h"
#include "U2Object.h"
#include "U2ThreadHeaders.h"


U2EG_NAMESPACE_BEGIN


class Worker : public Object
{
public:
	Worker(const String& type, const String& name = BLANK);

	template<class _Fn, class... _Args>
	void initialize(_Fn&& _Fx, _Args&&... _Ax)
	{
		m_Thread = std::move(std::thread(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...));
		m_szGuid = Worker::threadId2String(m_Thread.get_id());
	}

	void join();

	inline std::thread::id getThreadId() const;

	static String threadId2String(std::thread::id& tid);

protected:
	std::thread			m_Thread;
};



class WorkerManager : public SimpleObjectManager<Worker>, public Singleton < WorkerManager >
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
	Worker* createObject(_Fn&& _Fx, _Args&&... _Ax)
	{
		// Worker用SimpleObjectManager，
		// SimpleObjectManager应该改成guid为主键，name为辅键的形式，
		// 这样创建Worker时就不会改变key了，
		static size_t i = 0;
		StringStream ss;
		ss << i++;

		Worker* pObj = SimpleObjectManager<Worker>::createObject(GET_OBJECT_TYPE(Worker), ss.str());
		if (pObj != nullptr)
		{
			pObj->initialize(std::forward<_Fn>(_Fx), std::forward<_Args>(_Ax)...);
		}
		return pObj;
	}

protected:
	virtual Worker* createObject(const String& type, const String& name);

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
};


U2EG_NAMESPACE_END


#endif
