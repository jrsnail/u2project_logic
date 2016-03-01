#ifndef __U2ThreadDefinesCpp11_H__
#define __U2ThreadDefinesCpp11_H__


#define U2_AUTO_MUTEX               mutable std::mutex    U2_AUTO_MUTEX_NAME
#define U2_LOCK_AUTO_MUTEX          std::unique_lock<std::mutex> lck(U2_AUTO_MUTEX_NAME)

// Thread objects and related functions
#define U2_THREAD_TYPE              std::thread
#define U2_THREAD_CURRENT_ID        std::this_thread::get_id()
#define U2_THREAD_CREATE(name, worker) std::thread* name = U2_NEW_T(std::thread, MEMCATEGORY_GENERAL)(worker)
#define U2_THREAD_DESTROY(name)     U2_DELETE_T(name, thread, MEMCATEGORY_GENERAL)

// Utility
#define U2_THREAD_ID_TYPE           std::thread::id


#endif
