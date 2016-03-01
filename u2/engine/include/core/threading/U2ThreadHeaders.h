#ifndef __U2ThreadHeaders_H__
#define __U2ThreadHeaders_H__

#if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX // required to stop windows.h messing up std::min
#endif

#if U2_THREAD_PROVIDER == 2
    #include "U2ThreadHeadersCpp11.h"
#endif

#include "U2ThreadDefines.h"

#endif


