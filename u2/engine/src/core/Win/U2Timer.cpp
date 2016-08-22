#include "U2Timer.h"

#include "U2Bitwise.h"
#include <sys/timeb.h>


U2EG_NAMESPACE_USING


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
Timer::Timer()
#if U2_PLATFORM == U2_PLATFORM_WIN32
    : mTimerMask( 0 )
#endif
{
    reset();
}
//-------------------------------------------------------------------------
Timer::~Timer()
{
}
//-------------------------------------------------------------------------
bool Timer::setOption( const String & key, const void * val )
{
#if U2_PLATFORM == U2_PLATFORM_WIN32
    if ( key == "QueryAffinityMask" )
    {
        // Telling timer what core to use for a timer read
        DWORD newTimerMask = * static_cast < const DWORD * > ( val );

        // Get the current process core mask
        DWORD_PTR procMask;
        DWORD_PTR sysMask;
        GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

        // If new mask is 0, then set to default behavior, otherwise check
        // to make sure new timer core mask overlaps with process core mask
        // and that new timer core mask is a power of 2 (i.e. a single core)
        if( ( newTimerMask == 0 ) ||
            ( ( ( newTimerMask & procMask ) != 0 ) && Bitwise::isPO2( newTimerMask ) ) )
        {
            mTimerMask = newTimerMask;
            return true;
        }
    }
#endif

    return false;
}

//-------------------------------------------------------------------------
void Timer::reset()
{
#if U2_PLATFORM == U2_PLATFORM_WIN32
    // Get the current process core mask
    DWORD_PTR procMask;
    DWORD_PTR sysMask;
    GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

    // If procMask is 0, consider there is only one core available
    // (using 0 as procMask will cause an infinite loop below)
    if (procMask == 0)
        procMask = 1;

    // Find the lowest core that this process uses
    if( mTimerMask == 0 )
    {
        mTimerMask = 1;
        while( ( mTimerMask & procMask ) == 0 )
        {
            mTimerMask <<= 1;
        }
    }

    HANDLE thread = GetCurrentThread();

    // Set affinity to the first core
    DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);
#endif

    // Get the constant frequency
    QueryPerformanceFrequency(&mFrequency);

    // Query the timer
    QueryPerformanceCounter(&mStartTime);
    mStartTick = GetTickCount();

#if U2_PLATFORM == U2_PLATFORM_WIN32
    // Reset affinity
    SetThreadAffinityMask(thread, oldMask);
#endif

    mLastTime = 0;
    mZeroClock = clock();
}

//-------------------------------------------------------------------------
u2uint64 Timer::getMilliseconds()
{
    LARGE_INTEGER curTime;

#if U2_PLATFORM == U2_PLATFORM_WIN32
    HANDLE thread = GetCurrentThread();

    // Set affinity to the first core
    DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);
#endif

    // Query the timer
    QueryPerformanceCounter(&curTime);

#if U2_PLATFORM == U2_PLATFORM_WIN32
    // Reset affinity
    SetThreadAffinityMask(thread, oldMask);
#endif

    LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
    
    // scale by 1000 for milliseconds
    u2uint64 newTicks = (u2uint64) (1000 * newTime / mFrequency.QuadPart);

    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    u2uint64 check = GetTickCount() - mStartTick;
    u2int64 msecOff = (u2int64)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
        mStartTime.QuadPart += adjust;
        newTime -= adjust;

        // Re-calculate milliseconds
        newTicks = (u2uint64) (1000 * newTime / mFrequency.QuadPart);
    }

    // Record last time for adjust
    mLastTime = newTime;

    return newTicks;
}

//-------------------------------------------------------------------------
u2uint64 Timer::getMicroseconds()
{
    LARGE_INTEGER curTime;

#if U2_PLATFORM == U2_PLATFORM_WIN32
    HANDLE thread = GetCurrentThread();

    // Set affinity to the first core
    DWORD_PTR oldMask = SetThreadAffinityMask(thread, mTimerMask);
#endif

    // Query the timer
    QueryPerformanceCounter(&curTime);

#if U2_PLATFORM == U2_PLATFORM_WIN32
    // Reset affinity
    SetThreadAffinityMask(thread, oldMask);
#endif

    LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
    
    // get milliseconds to check against GetTickCount
    u2uint64 newTicks = (u2uint64) (1000 * newTime / mFrequency.QuadPart);
    
    // detect and compensate for performance counter leaps
    // (surprisingly common, see Microsoft KB: Q274323)
    u2uint64 check = GetTickCount() - mStartTick;
    u2int64 msecOff = (u2int64)(newTicks - check);
    if (msecOff < -100 || msecOff > 100)
    {
        // We must keep the timer running forward :)
        LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
        mStartTime.QuadPart += adjust;
        newTime -= adjust;
    }

    // Record last time for adjust
    mLastTime = newTime;

    // scale by 1000000 for microseconds
    u2uint64 newMicro = (u2uint64) (1000000 * newTime / mFrequency.QuadPart);

    return newMicro;
}

//-------------------------------------------------------------------------
u2uint64 Timer::getMillisecondsCPU()
{
    clock_t newClock = clock();
    return (u2uint64)( (float)( newClock - mZeroClock ) / ( (float)CLOCKS_PER_SEC / 1000.0 ) ) ;
}

//-------------------------------------------------------------------------
u2uint64 Timer::getMicrosecondsCPU()
{
    clock_t newClock = clock();
    return (u2uint64)( (float)( newClock - mZeroClock ) / ( (float)CLOCKS_PER_SEC / 1000000.0 ) ) ;
}