#ifndef __U2HomogenousDispatch_H__
#define __U2HomogenousDispatch_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MemoryAllocatorConfig.h"
#include "U2ThreadHeaders.h"
#include "U2ThreadPoolInterface.h"
#include "U2HomogenousQueue.h"
#include "U2GenericThreadPool.h"


U2EG_NAMESPACE_BEGIN


template<class Function>
class HomogenousDispatch
{
public:
	explicit HomogenousDispatch(int thread_count = -1,
		std::size_t queue_size = 0,
		std::size_t maxpart = 1)
		: queue(queue_size,
			(maxpart != 1) ? maxpart : 3 * (GenericThreadPool::determine_thread_count(thread_count) + 1))
		, pool(queue, thread_count)
	{ }

	virtual ~HomogenousDispatch()
	{
		wait();
		join();
	}

	template<class F>
	void run(F&& f)
	{
		queue.put(std::forward<F>(f));
	}

	void wait()
	{
		pool.help(true); 	// Help out instead of sitting around idly.
		queue.wait();
	}

	void join()
	{
		queue.shutdown();
		pool.join();
	}

	/**
	* Run a function on all members of a container
	*
	* @param container
	*			The container to process
	* @param fun
	*			The function taking one parameter
	*			by reference and returning void.
	*
	* Does not wait for all tasks to finish! Caller is
	* responsible for wait()ing on the pool if necessary.
	*/
	template<class Container, class F>
	F run_for_each(Container&& container, F&& fun)
	{
		for (auto& e : container)
		{
			run([&fun, &e]() {
				fun(e);
			});
		}
		return std::forward<F>(fun);
	}

protected:
	HomogenousQueue<Function> queue;
	GenericThreadPool pool;
};


U2EG_NAMESPACE_END


#endif
