#ifndef __AndroidTimer_H__
#define __AndroidTimer_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"


U2EG_NAMESPACE_BEGIN


/** Timer class */
class _U2Export Timer : public GeneralAllocatedObject
{
private:
    struct timeval start;
    clock_t zeroClock;

public:
    Timer();
    ~Timer();

    /** Method for setting a specific option of the Timer. These options are usually
        specific for a certain implementation of the Timer class, and may (and probably
        will) not exist across different implementations.  reset() must be called after
        all setOption() calls.
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
    bool setOption( const String& strKey, const void* pValue ) { return false; }

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
