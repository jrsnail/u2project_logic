#ifndef __U2MemoryTracker_H__
#define __U2MemoryTracker_H__


#include "U2Prerequisites.h"
//#include "U2STLRedefined.h"
#include "U2HeaderPrefix.h"


// Don't include prerequisites, can cause a circular dependency
// This file must be included within another file which already has the prerequisites in it
//#include "OgrePrerequisites.h"
#ifndef U2_COMPILER
#   pragma message "MemoryTracker included somewhere OgrePrerequisites.h wasn't!"
#endif

// If we're using the GCC 3.1 C++ Std lib
#if U2_COMPILER == U2_COMPILER_GNUC && U2_COMP_VER >= 310 && !defined(STLPORT)
// We need to define a hash function for void*
// For gcc 4.3 see http://gcc.gnu.org/gcc-4.3/changes.html
#   if U2_COMP_VER >= 430
#       include <tr1/unordered_map>
#   else
#       include <ext/hash_map>
namespace __gnu_cxx
{
    template <> struct hash< void* >
    {
        size_t operator()( void* const & ptr ) const
        {
            return (size_t)ptr;
        }
    };
}

#   endif
#endif

#if U2_MEMORY_TRACKER
#   include "threading/U2ThreadHeaders.h"
#endif


U2EG_NAMESPACE_BEGIN


/** \addtogroup Core
*  @{
*/
/** \addtogroup Memory
*  @{
*/

#if U2_MEMORY_TRACKER

/** This class tracks the allocations and deallocations made, and
    is able to report memory statistics and leaks.
@note
    This class is only available in debug builds.
*/
class _U2Export MemoryTracker
{
protected:
    U2_AUTO_MUTEX;

    // Allocation record
    struct Alloc
    {
        size_t bytes;
        unsigned int pool;
        std::string filename;
        size_t line;
        std::string function;

        Alloc() :bytes(0), line(0) {}
        Alloc(size_t sz, unsigned int p, const char* file, size_t ln, const char* func)
            :bytes(sz), pool(p), line(ln)
        {
            if (file)
                filename = file;
            if (func)
                function = func;
        }
    };

    std::string mLeakFileName;
    bool mDumpToStdOut;
    typedef U2_HashMap<void*, Alloc> AllocationMap;
    AllocationMap mAllocations;

    size_t mTotalAllocations;
    typedef std::vector<size_t> AllocationsByPool;
    AllocationsByPool mAllocationsByPool;
    bool mRecordEnable;

    void reportLeaks();

    // protected ctor
    MemoryTracker()
        : mLeakFileName("U2Leaks.log"), mDumpToStdOut(true),
        mTotalAllocations(0), mRecordEnable(true)
    {
    }

public:

    /** Set the name of the report file that will be produced on exit. */
    void setReportFileName(const std::string& name)
    {
        mLeakFileName = name;
    }
    /// Return the name of the file which will contain the report at exit
    const std::string& getReportFileName() const
    {
        return mLeakFileName;
    }
    /// Sets whether the memory report should be sent to stdout
    void setReportToStdOut(bool rep)
    {
        mDumpToStdOut = rep;
    }
    /// Gets whether the memory report should be sent to stdout
    bool getReportToStdOut() const
    {
        return mDumpToStdOut;
    }

    /// Get the total amount of memory allocated currently.
    size_t getTotalMemoryAllocated() const;
    /// Get the amount of memory allocated in a given pool
    size_t getMemoryAllocatedForPool(unsigned int pool) const;


    /** Record an allocation that has been made. Only to be called by
        the memory management subsystem.
        @param ptr The pointer to the memory
        @param sz The size of the memory in bytes
        @param pool The memory pool this allocation is occurring from
        @param file The file in which the allocation is being made
        @param ln The line on which the allocation is being made
        @param func The function in which the allocation is being made
    */
    void _recordAlloc(void* ptr, size_t sz, unsigned int pool = 0,
                        const char* file = 0, size_t ln = 0, const char* func = 0);
    /** Record the deallocation of memory. */
    void _recordDealloc(void* ptr);

    /// Sets whether the record alloc/dealloc enabled.
    void setRecordEnable(bool recordEnable)
    {
        mRecordEnable = recordEnable;
    }

    /// Gets whether the record alloc/dealloc enabled.
    bool getRecordEnable() const
    {
        return mRecordEnable;
    }

    ~MemoryTracker()
    {
        reportLeaks();

        // Disable recording so we don't try to record our own deallocation.
        mRecordEnable = false;
    }

    /// Static utility method to get the memory tracker instance
    static MemoryTracker& get();
};


#endif

/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif

