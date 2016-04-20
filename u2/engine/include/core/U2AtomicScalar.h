#ifndef __U2AtomicScalar_H__
#define __U2AtomicScalar_H__

#include "U2Prerequisites.h"
//#include "OgrePlatformInformation.h"
#include "U2Exception.h"

#if (((U2_COMPILER == U2_COMPILER_GNUC) && (U2_COMP_VER >= 412)) || (U2_COMPILER == U2_COMPILER_CLANG))

// Atomics are not yet supported for the unsigned long long int(ResourceHandle) type as of Clang 5.0. So only GCC for now.
#if ((U2_COMPILER == U2_COMPILER_GNUC) && (U2_COMP_VER >= 473))
    #define BUILTIN_FETCH_ADD(var, add) __atomic_fetch_add (var, add, __ATOMIC_SEQ_CST);
    #define BUILTIN_ADD_FETCH(var, add) __atomic_add_fetch (var, add, __ATOMIC_SEQ_CST);
    #define BUILTIN_SUB_FETCH(var, sub) __atomic_sub_fetch (var, sub, __ATOMIC_SEQ_CST);
#else
    #define BUILTIN_FETCH_ADD(var, add) __sync_fetch_and_add (var, add);
    #define BUILTIN_ADD_FETCH(var, add) __sync_add_and_fetch (var, add);
    #define BUILTIN_SUB_FETCH(var, sub) __sync_sub_and_fetch (var, sub);
#endif

U2EG_NAMESPACE_BEGIN

    /** \addtogroup Core
    *  @{
    */
    /** \addtogroup General
    *  @{
    */
    template<class T> class AtomicScalar
    {

        public:

        AtomicScalar (const T &initial)
            : mField(initial)
        {   }

        AtomicScalar (const AtomicScalar<T> &cousin)
            : mField(cousin.mField)
        {   }

        AtomicScalar () 
        {   }

        void operator= (const AtomicScalar<T> &cousin)
        {
            mField = cousin.mField;
        }

        T get (void) const
        {
            return mField;
        }

        void set (const T &v)
        {
            mField = v; 
        }   

        bool cas (const T &old, const T &nu)
        {
            return __sync_bool_compare_and_swap (&mField, old, nu);
        }
            
        T operator++ (void)
        {
            return BUILTIN_ADD_FETCH (&mField, 1);
        }
            
        T operator-- (void)
        {
            return BUILTIN_ADD_FETCH (&mField, -1);
        }

        T operator++ (int)
        {
            return BUILTIN_FETCH_ADD (&mField, 1);
        }
            
        T operator-- (int)
        {
            return BUILTIN_FETCH_ADD (&mField, -1);
        }

        T operator+=(const T &add)
        {
            return BUILTIN_ADD_FETCH (&mField, add);
        }

        T operator-=(const T &sub)
        {
            return BUILTIN_SUB_FETCH (&mField, sub);
        }

        // Need special alignment for atomic functions on ARM CPU's
#if U2_CPU == U2_CPU_ARM
#   if U2_COMPILER == U2_COMPILER_MSVC
        __declspec(align(16)) volatile T mField;
#   elif (U2_COMPILER == U2_COMPILER_GNUC) || (U2_COMPILER == U2_COMPILER_CLANG)
        volatile T mField __attribute__((__aligned__(16)));
#   endif
#elif U2_CPU == U2_CPU_X86 && U2_PLATFORM == U2_PLATFORM_APPLE && ((U2_COMPILER == U2_COMPILER_CLANG) && (U2_COMP_VER < 500))
/* https://ogre3d.atlassian.net/browse/OGRE-324
 * but see http://stackoverflow.com/questions/19387043/how-can-i-reliably-detect-the-version-of-clang-at-preprocessing-time
 * the version in U2_COMP_VER is exactly the version reported with 'gcc -v' (Apple LLVM version 5.0 would produce U2_COMP_VER==500)
 * but if the system built clang from sources that same compiler would be LLVM version 3.3 with U2_COMP_VER==330
 */
        volatile T mField __attribute__((__aligned__(16)));
#else
        volatile T mField;
#endif

    };
    /** @} */
    /** @} */

	U2EG_NAMESPACE_END


#elif U2_COMPILER == U2_COMPILER_MSVC && U2_COMP_VER >= 1400

#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	if !defined(NOMINMAX) && defined(_MSC_VER)
#		define NOMINMAX // required to stop windows.h messing up std::min
#	endif
#	include <windows.h>
#	include <intrin.h>
#	include "threading/U2ThreadHeaders.h"

// Save warnings state
#   pragma warning (push)
#   pragma warning (disable : 4244)



U2EG_NAMESPACE_BEGIN


    // a hack so we can support windows xp.
#define NEED_TO_INIT_INTERLOCKEDCOMPAREEXCHANGE64WRAPPER
struct _U2Export InterlockedCompareExchange64Wrapper
{
    InterlockedCompareExchange64Wrapper();

    typedef 
        LONGLONG
        (WINAPI *func_InterlockedCompareExchange64)( 
        __inout LONGLONG volatile *Destination,
        __in    LONGLONG Exchange,
        __in    LONGLONG Comperand) ;

    static func_InterlockedCompareExchange64 U2_InterlockedCompareExchange64;

    static FORCEINLINE
        LONGLONG
        U2_InterlockedIncrement64 (
        __inout LONGLONG volatile *Addend
        )
    {
        LONGLONG Old;

        do {
            Old = *Addend;
        } while (U2_InterlockedCompareExchange64(Addend,
            Old + 1,
            Old) != Old);

        return Old + 1;
    }

    static FORCEINLINE
        LONGLONG
        U2_InterlockedDecrement64 (
        __inout LONGLONG volatile *Addend
        )
    {
        LONGLONG Old;

        do {
            Old = *Addend;
        } while (U2_InterlockedCompareExchange64(Addend,
            Old - 1,
            Old) != Old);

        return Old - 1;
    }

};

/** \addtogroup Core
*  @{
*/
/** \addtogroup General
*  @{
*/
template<class T> class AtomicScalar
{
public:

    AtomicScalar (const T &initial)
        : mField(initial)
    {   }

    AtomicScalar (const AtomicScalar<T> &cousin)
        : mField(cousin.mField)
    {   }

    AtomicScalar () 
    {   }

    void operator= (const AtomicScalar<T> &cousin)
    {
        mField = cousin.mField;
    }

    T get (void) const
    {
        return mField;
    }

    void set (const T &v)
    {
        mField = v;
    }   

    bool cas (const T &old, const T &nu)
    {
        if (sizeof(T)==2) {
            return _InterlockedCompareExchange16((SHORT*)&mField, static_cast<SHORT>(nu), static_cast<SHORT>(old)) == static_cast<SHORT>(old);
        } 
        else if (sizeof(T)==4) 
        {
            return _InterlockedCompareExchange((LONG*)&mField, static_cast<LONG>(nu), static_cast<LONG>(old)) == static_cast<LONG>(old);
        } 
        else if (sizeof(T)==8 && InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64 != NULL) {
            return InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64((LONGLONG*)&mField, static_cast<LONGLONG>(nu), static_cast<LONGLONG>(old)) == static_cast<LONGLONG>(old);
        } 
        else {
            U2_LOCK_AUTO_MUTEX;
            if (mField != old) return false;
            mField = nu;
            return true;
        }
    }
            
    T operator++ (void)
    {
        if (sizeof(T)==2) {
            return _InterlockedIncrement16((SHORT*)&mField);
        } else if (sizeof(T)==4) {
            return InterlockedIncrement((LONG*)&mField);
        } else if (sizeof(T)==8 && InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64 != NULL) {
            return InterlockedCompareExchange64Wrapper::U2_InterlockedIncrement64((LONGLONG*)&mField);
        } else {
            U2_LOCK_AUTO_MUTEX;
            return ++mField;
        }
    }
            
    T operator-- (void)
    {
        if (sizeof(T)==2) {
            return _InterlockedDecrement16((SHORT*)&mField);
        } else if (sizeof(T)==4) {
            return InterlockedDecrement((LONG*)&mField);
        } else if (sizeof(T)==8 && InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64 != NULL) {
            return InterlockedCompareExchange64Wrapper::U2_InterlockedDecrement64((LONGLONG*)&mField);
        } else {
            U2_LOCK_AUTO_MUTEX;
            return --mField;
        }
    }

    T operator++ (int)
    {
        if (sizeof(T)==2) {
            return _InterlockedIncrement16((SHORT*)&mField)-1;
        } else if (sizeof(T)==4) {
            return InterlockedIncrement((LONG*)&mField)-1;
        } else if (sizeof(T)==8 && InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64 != NULL) {
            return InterlockedCompareExchange64Wrapper::U2_InterlockedIncrement64((LONGLONG*)&mField)-1;
        } else {
            U2_LOCK_AUTO_MUTEX;
            return mField++;
        }
    }
            
    T operator-- (int)
    {
        if (sizeof(T)==2) {
            return _InterlockedDecrement16((SHORT*)&mField)+1;
        } else if (sizeof(T)==4) {
            return InterlockedDecrement((LONG*)&mField)+1;
        } else if (sizeof(T)==8 && InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64 != NULL) {
            return InterlockedCompareExchange64Wrapper::U2_InterlockedDecrement64((LONGLONG*)&mField)+1;
        } else {
            U2_LOCK_AUTO_MUTEX;
            return mField--;
        }
    }

    T operator+=(const T &add)
    {
        if ((sizeof(T)==2) || (sizeof(T)==4) || (sizeof(T)==8 && InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64 != NULL)) {
            //The function InterlockedExchangeAdd is not available for 64 and 16 bit version
            //We will use the cas operation instead. 
            T newVal;
            do {
                //Create a value of the current field plus the added value
                newVal = mField + add;
                //Replace the current field value with the new value. Ensure that the value 
                //of the field hasn't changed in the mean time by comparing it to the new value
                //minus the added value. 
            } while (!cas(newVal - add, newVal)); //repeat until successful
            return newVal;
        }
        else
        {
            U2_LOCK_AUTO_MUTEX;
            mField += add;
            return mField;
        }
    }

    T operator-=(const T &sub)
    {
        if ((sizeof(T)==2) || (sizeof(T)==4) || (sizeof(T)==8 && InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64 != NULL)) {
            //The function InterlockedExchangeAdd is not available for 64 and 16 bit version
            //We will use the cas operation instead. 
            T newVal;
            do {
                //Create a value of the current field plus the added value
                newVal = mField - sub;
                //Replace the current field value with the new value. Ensure that the value 
                //of the field hasn't changed in the mean time by comparing it to the new value
                //minus the added value. 
            } while (!cas(newVal + sub, newVal)); //repeat until successful
            return newVal;
        }
        else
        {
            U2_LOCK_AUTO_MUTEX;
            mField -= sub;
            return mField;
        }
    }

protected:
    U2_AUTO_MUTEX;
    volatile T mField;

};
/** @} */
/** @} */

U2EG_NAMESPACE_END

#   pragma warning (pop)

#else

#	include "threading/U2ThreadHeaders.h"

U2EG_NAMESPACE_BEGIN

/** \addtogroup Core
*  @{
*/
/** \addtogroup General
*  @{
*/
template <class T> class AtomicScalar {

    public:

    AtomicScalar (const T &initial)
        : mField(initial)
    {   }

    AtomicScalar (const AtomicScalar<T> &cousin)
        : mField(cousin.mField)
    {   }

    AtomicScalar ()
    {   }

    void operator= (const AtomicScalar<T> &cousin)
    {
        mField = cousin.mField;
    }

    T get (void) const
    {
        // no lock required here
        // since get will not interfere with set or cas
        // we may get a stale value, but this is ok
        return mField;
    }

    void set (const T &v)
    {
        mField = v;
    }

    bool cas (const T &old, const T &nu)
    {
        U2_LOCK_AUTO_MUTEX;
        if (mField != old) return false;
        mField = nu;
        return true;
    }

    T operator++ (void)
    {
        U2_LOCK_AUTO_MUTEX;
        return ++mField;
    }

    T operator-- (void)
    {
        U2_LOCK_AUTO_MUTEX;
        return --mField;
    }

    T operator++ (int)
    {
        U2_LOCK_AUTO_MUTEX;
        return mField++;
    }

    T operator-- (int)
    {
        U2_LOCK_AUTO_MUTEX;
        return mField--;
    }

    T operator+=(const T &add)
    {
        U2_LOCK_AUTO_MUTEX;
        mField += add;
        return mField;
    }

    T operator-=(const T &sub)
    {
        U2_LOCK_AUTO_MUTEX;
        mField -= sub;
        return mField;
    }

    protected:

    U2_AUTO_MUTEX;

    volatile T mField;

};
/** @} */
/** @} */

U2EG_NAMESPACE_END

#endif

#endif

