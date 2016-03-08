#ifndef __U2ThreadHeaders_H__
#define __U2ThreadHeaders_H__

#if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX // required to stop windows.h messing up std::min
#endif


#include <thread>
#include <mutex>
#include <atomic>
#include <future>




#define U2_AUTO_MUTEX_NAME          m_mtx
#define U2_AUTO_MUTEX               mutable std::mutex    U2_AUTO_MUTEX_NAME
#define U2_LOCK_AUTO_MUTEX          std::unique_lock<std::mutex> lck(U2_AUTO_MUTEX_NAME)

// Thread objects and related functions
#define U2_THREAD_TYPE              std::thread
#define U2_THREAD_CURRENT_ID        std::this_thread::get_id()

// Utility
#define U2_THREAD_ID_TYPE           std::thread::id



#endif


