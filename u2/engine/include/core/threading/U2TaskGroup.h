#ifndef __U2TaskGroup_H__
#define __U2TaskGroup_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2MemoryAllocatorConfig.h"
#include "U2ThreadHeaders.h"
#include "U2ThreadPoolInterface.h"
#include "U2HomogenousDispatch.h"


U2EG_NAMESPACE_BEGIN


class TaskGroup : public TaskGroupInterface
{
protected:

	/**
	* Store pointers into the queue. Decorate the pointers
	* with an operator() to make them callable as needed by
	* HomogenousThreadPool.
	*
	* I tried std::unique_ptr but at least with g++ it was
	* very slow. Seems to do some heavyweight locking. Using
	* raw pointers, we can just delete them in the tasks
	* operator() or in the destructor. This is even more
	* flexible than using std::unique_ptr, there may be use
	* cases where the task shall outlive the execution by the
	* thread pool, for example for cleanup work to be done.
	*
	* Delete copy-constructor and copy-assignment so `pimpl`
	* is not deleted twice. The move constructor makes sure
	* to leave an empty `pimpl` behind.
	*/
	class QueueElement
	{
	protected:
		ThreadPoolTask* m_pTask;

		QueueElement() = delete;
		QueueElement(const QueueElement&) = delete;
		QueueElement& operator=(const QueueElement&) = delete;
		QueueElement& operator=(QueueElement&&) = delete;

	public:
		QueueElement(ThreadPoolTask* t) : m_pTask(t) { }
		QueueElement(QueueElement&& x) : m_pTask(x.m_pTask) { x.m_pTask = nullptr; }
		~QueueElement() { if (m_pTask) delete m_pTask; }

		void operator()() { (*m_pTask)(); m_pTask = nullptr; }
	};

protected:

	HomogenousDispatch<QueueElement> impl;

public:

	explicit TaskGroup(int thread_count = -1,
		std::size_t queue_size = 0,
		std::size_t maxpart = 1)
		: impl(thread_count, queue_size, maxpart)
	{
	}

	void run(std::unique_ptr<ThreadPoolTask>&& t) override
	{
		impl.run(t.release());
	}

	void run(ThreadPoolTask* t) override
	{
		impl.run(t);
	}


	/**
	* Wrap void functions in a task and run them without
	* exception handling.
	*/
	template<class Function>
	typename std::enable_if<!std::is_pointer<typename std::remove_reference<Function>::type>::value &&
		std::is_void<decltype(std::declval<typename std::remove_pointer<typename std::remove_reference<Function>::type>::type>()())>::value
	>::type run(Function&& f)
	{

		typedef typename std::remove_reference<Function>::type function_type;

		class WrappedFunction : public Task
		{
			Function f;
		public:
			WrappedFunction(function_type&& f) : f(std::move(f)) { }
			virtual void operator()() override
			{
				f();
				delete this;
			}
		};

		run(new WrappedFunction(std::forward<Function>(f)));
	}

	/**
	* For functions with nonvoid return type, catch exceptions
	* and return a future.
	*/
	template<class Function>
	typename std::enable_if<!std::is_pointer<typename std::remove_reference<Function>::type>::value &&
		!std::is_void<decltype(std::declval<typename std::remove_pointer<typename std::remove_reference<Function>::type>::type>()())>::value,
		std::future<decltype(std::declval<typename std::remove_pointer<typename std::remove_reference<Function>::type>::type>()())>
	>::type run(Function&& f)
	{
		typedef typename std::remove_reference<Function>::type function_type;
		typedef typename std::result_of<Function()>::type return_type;

		class WrappedFunction : public Task
		{
			Function f;
			std::promise<return_type> promise;

		public:
			WrappedFunction(function_type&& f) : f(std::move(f)) { }
			WrappedFunction(function_type& f) : f(f) { }

			std::future<return_type> get_future()
			{
				return promise.get_future();
			}

			virtual void operator()() override
			{
				try
				{
					promise.set_value(f());
				}
				catch (...)
				{
					promise.set_exception(std::current_exception());
				}
				delete this;
			}
		};

		WrappedFunction* task(new WrappedFunction(std::forward<Function>(f)));
		std::future<return_type> future(task->get_future());
		run(task);
		return future;
	}


	/**
	* Run a function on all objects in an iterator range
	*
	* @param first
	*			Start of the range
	*
	* @param last
	*			End of the range
	*
	* @param fun
	*			The function taking one parameter
	*			by reference and returning void.
	*
	* Does not wait for all tasks to finish! Caller is
	* responsible for wait()ing on the pool if necessary.
	*/
	template<class Iterator, class Function>
	void for_each(Iterator first, const Iterator& last, Function&& fun)
	{
		while (first != last)
		{
			Wrap<typename iterval_traits<Iterator>::type> e(iterval_traits<Iterator>::copy(first));
			++first;
			run([&fun, e]() {
				fun(iterval_traits<Iterator>::pass(std::move(e.value)));
			});;
		}
	}


	/**
	* Run a function on all members of a container
	*
	* @param container
	*			The container to process
	*
	* @param fun
	*			The function taking one parameter
	*			by reference and returning void.
	*
	* Does not wait for all tasks to finish! Caller is
	* responsible for wait()ing on the pool if necessary.
	*/
	template<class Container, class Function>
	void for_each(Container&& container, Function&& fun)
	{
		for (auto& e : container)
		{
			run([&fun, &e]() {
				fun(e);
			});
		}
	}

	void wait() override { impl.wait(); }

	void join() override { impl.join(); }
};


U2EG_NAMESPACE_END


#endif
