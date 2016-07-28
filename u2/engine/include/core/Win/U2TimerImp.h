#ifndef __WinTimerImp_H__
#define __WinTimerImp_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX // required to stop windows.h messing up std::min
#endif
#include <windows.h>


U2EG_NAMESPACE_BEGIN


class _U2Export Timer : public GeneralAllocatedObject
{
private:
    clock_t mZeroClock;

    DWORD mStartTick;
    LONGLONG mLastTime;
    LARGE_INTEGER mStartTime;
    LARGE_INTEGER mFrequency;

#if U2_PLATFORM == U2_PLATFORM_WIN32
    DWORD_PTR mTimerMask;
#endif
#if U2_PLATFORM == U2_PLATFORM_WINRT
    DWORD GetTickCount() { return (DWORD)GetTickCount64(); }
#endif

public:
    /** Timer constructor.  MUST be called on same thread that calls getMilliseconds() */
    Timer();
    ~Timer();

    /** Method for setting a specific option of the Timer. These options are usually
        specific for a certain implementation of the Timer class, and may (and probably
        will) not exist across different implementations.  reset() must be called after
        all setOption() calls.
        @par
        Current options supported are:
        <ul><li>"QueryAffinityMask" (DWORD): Set the thread affinity mask to be used
        to check the timer. If 'reset' has been called already this mask should
        overlap with the process mask that was in force at that point, and should
        be a power of two (a single core).</li></ul>
        @param
            strKey The name of the option to set
        @param
            pValue A pointer to the value - the size should be calculated by the timer
            based on the key
        @return
            On success, true is returned.
        @par
            On failure, false is returned.
    */
    bool setOption( const String& strKey, const void* pValue );

    /** Resets timer */
    void reset();

    /** Returns milliseconds since initialisation or last reset */
    u2uint64 getMilliseconds();

    /** Returns microseconds since initialisation or last reset */
    u2uint64 getMicroseconds();

    /** Returns milliseconds since initialisation or last reset, only CPU time measured */
    u2uint64 getMillisecondsCPU();

    /** Returns microseconds since initialisation or last reset, only CPU time measured */
    u2uint64 getMicrosecondsCPU();
};


U2EG_NAMESPACE_END

#endif
