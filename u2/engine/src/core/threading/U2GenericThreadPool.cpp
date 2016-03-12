//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2GenericThreadPool.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
GenericThreadPool::GenericThreadPool(ThreadPoolQueue& queue, int thread_count)
	: pending_exception(nullptr)
	, queue(queue)
	, thread_count(determine_thread_count(thread_count))
	, workers(this->thread_count)
{
	for (Worker* w : workers)
	{
		//std::thread t = std::move(std::thread(std::bind(&GenericThreadPool::_work, this)));
		w = WorkerManager::getSingleton().createObject(std::bind(&GenericThreadPool::_work, this));
	}	
}
//-----------------------------------------------------------------------
GenericThreadPool::~GenericThreadPool()
{
	// Abort processing if destructor runs during exception handling.
	if (std::uncaught_exception())
		queue.shutdown();

	join(); // Running threads would continue to access the destructed pool.
}
//-----------------------------------------------------------------------
void GenericThreadPool::help(bool return_if_idle)
{
	if (ignore_thread_pool_exceptions())
	{
		queue.work(return_if_idle);
	}
	else
	{
		try
		{
			queue.work(return_if_idle);
		}
		catch (...)
		{
			{
				std::exception_ptr e = std::current_exception();
				std::lock_guard<std::mutex> lock(mutex);
				if (!pending_exception)
					pending_exception = std::move(e);
			}
			queue.shutdown();
		}
	}
}
//-----------------------------------------------------------------------
void GenericThreadPool::rethrow_exception()
{
	if (pending_exception && !std::uncaught_exception())
	{
		queue.shutdown();
		_join_workers();
		if (!std::uncaught_exception())
		{
			std::exception_ptr e = pending_exception;
			pending_exception = nullptr;
			std::rethrow_exception(std::move(e));
		}
	}
}
//-----------------------------------------------------------------------
void GenericThreadPool::join()
{
	_join_workers();
	rethrow_exception();
}
//-----------------------------------------------------------------------
unsigned int GenericThreadPool::hardware_concurrency(int c)
{
	static int cached_concurrency = -1;
	if (c != -1)
		cached_concurrency = c;
	if (cached_concurrency == -1)
		cached_concurrency = std::thread::hardware_concurrency();
	return cached_concurrency;
}
//-----------------------------------------------------------------------
unsigned int GenericThreadPool::determine_thread_count(int thread_count)
{
	if (thread_count == -1
		&& !(thread_count = hardware_concurrency()))
	{
		thread_count = 8;
	}

	return thread_count;
}
//-----------------------------------------------------------------------
bool GenericThreadPool::ignore_thread_pool_exceptions(bool i)
{
	static bool do_ignore_exceptions = false;
	if (i)
		do_ignore_exceptions = i;
	return do_ignore_exceptions;
}
//-----------------------------------------------------------------------
void GenericThreadPool::_work()
{
	help(false);
}
//-----------------------------------------------------------------------
void GenericThreadPool::_join_workers()
{
	_work();		// Instead of hanging around, help the workers!
	for (Worker* w : workers)
		w->join();
}