#ifndef __U2ThreadDefines_H__
#define __U2ThreadDefines_H__


#define U2_AUTO_MUTEX_NAME          m_mtx


#if U2_THREAD_PROVIDER == 0
#   include "U2ThreadDefinesNone.h"
#elif U2_THREAD_PROVIDER == 2
#   include "U2ThreadDefinesCpp11.h"
#endif


#endif

