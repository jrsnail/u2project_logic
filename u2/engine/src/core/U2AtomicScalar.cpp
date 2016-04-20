#include "U2AtomicScalar.h"

#ifdef NEED_TO_INIT_INTERLOCKEDCOMPAREEXCHANGE64WRAPPER

U2EG_NAMESPACE_BEGIN

InterlockedCompareExchange64Wrapper::func_InterlockedCompareExchange64 InterlockedCompareExchange64Wrapper::U2_InterlockedCompareExchange64;

InterlockedCompareExchange64Wrapper::InterlockedCompareExchange64Wrapper()
{
    U2_InterlockedCompareExchange64 = NULL;

// In win32 we will get InterlockedCompareExchange64 function address, in XP we will get NULL - as the function doesn't exist there. 
#if (U2_PLATFORM == U2_PLATFORM_WIN32)
    HINSTANCE kernel32Dll = LoadLibrary("KERNEL32.DLL");
    U2_InterlockedCompareExchange64 = (func_InterlockedCompareExchange64)GetProcAddress(kernel32Dll, "InterlockedCompareExchange64");
#endif

// In WinRT we can't LoadLibrary("KERNEL32.DLL") - but on the other hand we don't have the issue - as InterlockedCompareExchange64 exist on
// all WinRT platforms (if not - add to the #if below)
#if (U2_PLATFORM == U2_PLATFORM_WINRT)
    U2_InterlockedCompareExchange64 = _InterlockedCompareExchange64;
#endif    
}

// Here we call the InterlockedCompareExchange64Wrapper constructor so U2_InterlockedCompareExchange64 will be init once when the program loads.
InterlockedCompareExchange64Wrapper interlockedCompareExchange64Wrapper;

U2EG_NAMESPACE_END

#endif