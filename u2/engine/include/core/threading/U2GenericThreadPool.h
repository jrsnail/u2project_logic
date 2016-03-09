#ifndef __U2GenericThreadPool_H__
#define __U2GenericThreadPool_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MemoryAllocatorConfig.h"
#include "U2ThreadPoolInterface.h"
#include "U2Worker.h"
#include "U2ThreadHeaders.h"



U2EG_NAMESPACE_BEGIN


class GenericThreadPool : public ThreadPool
{
	/*
protected:
	class Worker
	{
	protected:
		std::thread thread;

	public:
		Worker& operator=(std::thread&& t) { thread = std::move(t); return *this; }
		void join() { if (thread.joinable()) thread.join(); }
		std::thread::id getThreadId() { return thread.get_id(); };
	};
	*/

public:
	/**
	* Generic thread pool.
	*
	* As soon as the pool is created the threads start running
	* tasks from the queue until the queue is empty. When the
	* queue is empty the threads return and are ready to be
	* collected by join() or by the destructor.
	*
	* @param queue
	*			The queue delivering the tasks.
	*
	* @param thread_count The number of threads
	*			to use. If the thread count is not
	*			specified it defaults to the number of
	*			available hardware threads
	*			std::thread::hardware_concurrency(),
	*			as read through hardware_concurrency().
	*
	*/
	GenericThreadPool(ThreadPoolQueue& queue, int thread_count);

	/**
	* Destroy the thread pool.
	*
	* Generally a destructor should not wait for a long time,
	* and it should not throw any exceptions. Unfortunately
	* threads are not abortable in C++.  The only way to make
	* sure the threads have terminated is to wait for them to
	* return by themselves.  If we would allow the
	* destruction of the thread pool before the threads have
	* returned, the threads would continue to access the
	* memory of the destroyed thread pool, potentially
	* clobbering other objects residing in the recycled
	* memory.  We could allocate parts of the memory with
	* new, and leave it behind for the threads after the
	* thread pool is destructed.  But even then, the user
	* supplied functions run by the threads might access
	* memory that gets destroyed if the function that
	* constructed the thread pool terminates.  The danger of
	* undetected and undebuggable memory corruption is just
	* too big.
	*
	* With regard to the exceptions rethrown in the
	* destructor, it is better to signal the exception than
	* to ignore it silently.
	*
	* If it is not acceptable for the destructor to wait or
	* to throw an exception, just call join() before the pool
	* is destructed.  After join() the destructor is
	* guaranteed to run fast and without exceptions.
	*
	* If it should really be necessary to keep threads
	* running after the function that created the thread pool
	* returns, just create the thread pool on the heap with
	* new. And if you want to make sure nobody destroys the
	* thread pool, feel free to throw away the handle.
	*/
	virtual ~GenericThreadPool();

	/**
	* Help with the work.
	*
	* @param return_if_idle
	*		Never wait for work, return instead.
	*/
	void help(bool return_if_idle) override;

	/**
	* Rethrow a potentially pending exception from a worker
	* thread.
	*/
	void rethrow_exception() override;

	/**
	* Wait for all threads to finish and collect them.
	*
	* Leaves the thread pool ready for destruction.
	*/
	void join() override;

	/**
	* Cache the hardware concurrency so we are sure that it
	* is cheap to get. Also this gives us a point to
	* cheat. The cached value can be modified by a parameter.
	*
	* @param c
	*		The hardware concurrency to use
	*/
	static unsigned int hardware_concurrency(int c = -1);

	/**
	* Determine thread count to use based on users
	* specifications.
	*
	* @param thread_count
	*		Runtime specified threadcount parameter.
	*
	* @returns
	*		The number of threads to use.
	*
	* This policy function does just some
	* guesswork. Allocating a number of threads in the order
	* of the hardware threads may be a good bet for CPU-bound
	* work. For other tasks it depends.
	*/
	static unsigned int determine_thread_count(int thread_count = -1);

	/**
	* Switch exception handling off
	*/
	static bool ignore_thread_pool_exceptions(bool i = true);

protected:
	// Copying and moving are not supported.
	GenericThreadPool(const GenericThreadPool&) = delete;
	GenericThreadPool(GenericThreadPool&&) = delete;
	GenericThreadPool& operator=(const GenericThreadPool&) = delete;
	GenericThreadPool& operator=(GenericThreadPool&&) = delete;

	/**
	* The main function of the thread.
	*/
	void _work();

	/**
	* Wait for all workers to finish.
	*/
	void _join_workers();

protected:
	std::mutex mutex;
	std::exception_ptr pending_exception;
	ThreadPoolQueue& queue;
	const unsigned int thread_count; /// The number of threads
	std::vector<Worker*> workers;

};


U2EG_NAMESPACE_END


#endif
