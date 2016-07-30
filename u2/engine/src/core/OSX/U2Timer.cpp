#include "U2Timer.h"

#include <sys/time.h>


U2EG_NAMESPACE_USING


//--------------------------------------------------------------------------------//
Timer::Timer()
{
    reset();
}

//--------------------------------------------------------------------------------//
Timer::~Timer()
{
}

//--------------------------------------------------------------------------------//
void Timer::reset()
{
    zeroClock = clock();
    gettimeofday(&start, NULL);
}
//--------------------------------------------------------------------------------//
u2uint64 Timer::getMilliseconds()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000;
}
//--------------------------------------------------------------------------------//
u2uint64 Timer::getMicroseconds()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec - start.tv_sec) * 1000000 + (now.tv_usec - start.tv_usec);
}
//-- Common Across All Timers ----------------------------------------------------//
u2uint64 Timer::getMillisecondsCPU()
{
    clock_t newClock = clock();
    return (u2uint64)((float)(newClock - zeroClock) / ((float)CLOCKS_PER_SEC / 1000.0)) ;
}
//-- Common Across All Timers ----------------------------------------------------//
u2uint64 Timer::getMicrosecondsCPU()
{
    clock_t newClock = clock();
    return (u2uint64)((float)(newClock - zeroClock) / ((float)CLOCKS_PER_SEC / 1000000.0)) ;
}
