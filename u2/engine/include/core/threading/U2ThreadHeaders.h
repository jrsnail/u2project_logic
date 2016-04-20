#ifndef __U2ThreadHeaders_H__
#define __U2ThreadHeaders_H__

#if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX // required to stop windows.h messing up std::min
#endif


#include <thread>
#include <mutex>
#include <atomic>
#include <future>



// lock
#define U2_AUTO_MUTEX_NAME          m_mtx
#define U2_AUTO_MUTEX               mutable std::mutex		U2_AUTO_MUTEX_NAME
#define U2_STATIC_MUTEX(name)		static std::mutex name
#define U2_STATIC_MUTEX_INSTANCE(name)	std::mutex name
#define U2_LOCK_AUTO_MUTEX          std::unique_lock<std::mutex> autolck(U2_AUTO_MUTEX_NAME)
#define U2_MUTEX(name)				mutable std::mutex		name
#define U2_LOCK_MUTEX(name)			std::unique_lock<std::mutex> lck(name)
#define U2_LOCK_MUTEX_NAMED(mutexName, lockName)	std::unique_lock<std::mutex> lockName(mutexName)

// condition variable
#define U2_THREAD_SYNCHRONISER(sync)		std::condition_variable sync
#define U2_THREAD_WAIT(sync, lock)			sync.wait(lock)
#define U2_THREAD_WAIT_FOR(sync, lock, millisecs)	sync.wait_for(lock, std::chrono::milliseconds(millisecs))
#define U2_THREAD_NOTIFY_ONE(sync)			sync.notify_one()
#define U2_THREAD_NOTIFY_ALL(sync)			sync.notify_all()


// Thread objects and related functions
#define U2_THREAD_TYPE              std::thread
#define U2_THREAD_CURRENT_ID        std::this_thread::get_id()

// Utility
#define U2_THREAD_ID_TYPE           std::thread::id



#endif


