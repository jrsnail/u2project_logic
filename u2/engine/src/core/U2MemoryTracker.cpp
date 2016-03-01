#include "U2MemoryTracker.h"

#include "U2STLRedefined.h"


#if U2_PLATFORM == U2_PLATFORM_WIN32 || U2_PLATFORM == U2_PLATFORM_WINRT
#   include <windows.h>
#   define U2_OutputCString(str) ::OutputDebugStringA(str)
#   define U2_OutputWString(str) ::OutputDebugStringW(str)
#else
#   define U2_OutputCString(str) std::cerr << str
#   define U2_OutputWString(str) std::cerr << str
#endif


U2EG_NAMESPACE_USING

    
#if U2_MEMORY_TRACKER
//--------------------------------------------------------------------------
MemoryTracker& MemoryTracker::get()
{
    static MemoryTracker tracker;
    return tracker;
}
//--------------------------------------------------------------------------
void MemoryTracker::_recordAlloc(void* ptr, size_t sz, unsigned int pool, 
                    const char* file, size_t ln, const char* func)
{
    if (mRecordEnable)
    {
        U2_LOCK_AUTO_MUTEX;

        assert(mAllocations.find(ptr) == mAllocations.end() && "Double allocation with same address - "
            "this probably means you have a mismatched allocation / deallocation style, "
            "check if you're are using U2_ALLOC_T / U2_FREE and U2_NEW_T / U2_DELETE_T consistently");

        mAllocations[ptr] = Alloc(sz, pool, file, ln, func);
        if(pool >= mAllocationsByPool.size())
            mAllocationsByPool.resize(pool+1, 0);
        mAllocationsByPool[pool] += sz;
        mTotalAllocations += sz;
    }
}
//--------------------------------------------------------------------------
void MemoryTracker::_recordDealloc(void* ptr)
{
    if (mRecordEnable)
    {
        // deal cleanly with null pointers
        if (!ptr)
            return;

        U2_LOCK_AUTO_MUTEX;

        AllocationMap::iterator i = mAllocations.find(ptr);
        assert(i != mAllocations.end() && "Unable to locate allocation unit - "
            "this probably means you have a mismatched allocation / deallocation style, "
            "check if you're are using U2_ALLOC_T / U2_FREE and U2_NEW_T / U2_DELETE_T consistently");
        // update category stats
        mAllocationsByPool[i->second.pool] -= i->second.bytes;
        // global stats
        mTotalAllocations -= i->second.bytes;
        mAllocations.erase(i);
    }
}   
//--------------------------------------------------------------------------
size_t MemoryTracker::getTotalMemoryAllocated() const
{
    return mTotalAllocations;
}
//--------------------------------------------------------------------------
size_t MemoryTracker::getMemoryAllocatedForPool(unsigned int pool) const
{
    return mAllocationsByPool[pool];
}
//--------------------------------------------------------------------------
void MemoryTracker::reportLeaks()
{   
    if (mRecordEnable)
    {
        //u2::StringStream os;
        std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> > os;

        if (mAllocations.empty())
        {
            os << "U2 Memory: No memory leaks" << std::endl;
        }
        else
        {           
            os << "U2 Memory: Detected memory leaks !!! " << std::endl;
            os << "U2 Memory: (" << mAllocations.size() << ") Allocation(s) with total " << mTotalAllocations << " bytes." << std::endl;
            os << "U2 Memory: Dumping allocations -> " << std::endl;


            for (AllocationMap::const_iterator i = mAllocations.begin(); i != mAllocations.end(); ++i)
            {
                const Alloc& alloc = i->second;
                if (!alloc.filename.empty())                
                    os << alloc.filename;
                else
                    os << "(unknown source):";

                os << "(" << alloc.line << ") : {" << alloc.bytes << " bytes}" << " function: " << alloc.function << std::endl;                 

            }           
            os << std::endl;            
        }

        if (mDumpToStdOut)        
            U2_OutputCString(os.str().c_str());

        std::ofstream of;
        of.open(mLeakFileName.c_str());
        of << os.str();
        of.close();
    }
}
#endif // U2_DEBUG_MODE   
    