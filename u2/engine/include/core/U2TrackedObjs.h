#ifndef __U2TrackedObjs_H__
#define __U2TrackedObjs_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN



class _U2Export Location : public Object
{
public:
    typedef map<String, String>::type   InfoMap;

public:
    // Constructor should be called with a long-lived char*, such as __FILE__.
    // It assumes the provided value will persist as a global constant, and it
    // will not make a copy of it.
    Location(const char* function_name,
        const char* file_name,
        int line_number,
        const void* program_counter);

    // Provide a default constructor for easy of debugging.
    Location();

    // Comparison operator for insertion into a std::map<> hash tables.
    // All we need is *some* (any) hashing distinction.  Strings should already
    // be unique, so we don't bother with strcmp or such.
    // Use line number as the primary key (because it is fast, and usually gets us
    // a difference), and then pointers as secondary keys (just to get some
    // distinctions).
    bool operator < (const Location& other) const 
    {
        if (line_number_ != other.line_number_)
            return line_number_ < other.line_number_;
        if (file_name_ != other.file_name_)
            return file_name_ < other.file_name_;
        return function_name_ < other.function_name_;
    }

    const char* function_name()   const { return function_name_; }
    const char* file_name()       const { return file_name_; }
    int line_number()             const { return line_number_; }
    const void* program_counter() const { return program_counter_; }

    String ToString() const;

    // Translate the some of the state in this instance into a human readable
    // string with HTML characters in the function names escaped, and append that
    // string to |output|.  Inclusion of the file_name_ and function_name_ are
    // optional, and controlled by the boolean arguments.
    String WriteAsHtml(bool display_filename, bool display_function_name);

    // Write function_name_ in HTML with '<' and '>' properly encoded.
    String WriteFunctionNameAsHtml();

    // Construct a Value* representation.  The caller assumes ownership of the
    // memory in the returned instance.
    const InfoMap& ToValue();

private:
    const char* function_name_;
    const char* file_name_;
    int line_number_;
    const void* program_counter_;
    InfoMap    m_LocationInfo;
};

_U2Export const void* GetProgramCounter();

// Define a macro to record the current source location.
#define FROM_HERE                 FROM_HERE_WITH_EXPLICIT_FUNCTION(__FUNCTION__)

#define FROM_HERE_WITH_EXPLICIT_FUNCTION(function_name)                        \
    ::u2::Location(function_name,                                              \
                   __FILE__,                                                   \
                   __LINE__,                                                   \
                   ::u2::GetProgramCounter())




// For a specific thread, and a specific birth place, the collection of all
// death info (with tallies for each death thread, to prevent access conflicts).
class ThreadData;
class _U2Export BirthOnThread
{
public:
    typedef map<String, String>::type   InfoMap;

public:
    BirthOnThread(const Location& location, const ThreadData& current);

    const Location location() const;
    const ThreadData* birth_thread() const;

    // Insert our state (location, and thread name) into the dictionary.
    // Use the supplied |prefix| in front of "thread_name" and "location"
    // respectively when defining keys.
    //void ToValue(const String& prefix, InfoMap* dictionary) const;

private:
    // File/lineno of birth.  This defines the essence of the task, as the context
    // of the birth (construction) often tell what the item is for.  This field
    // is const, and hence safe to access from any thread.
    const Location location_;

    // The thread that records births into this object.  Only this thread is
    // allowed to update birth_count_ (which changes over time).
    const ThreadData* const birth_thread_;

    InfoMap         m_InfoMap;

    DISALLOW_COPY_AND_ASSIGN(BirthOnThread);
};


// A class for accumulating counts of births (without bothering with a map<>).
class _U2Export Births : public BirthOnThread 
{
public:
    Births(const Location& location, const ThreadData& current);

    inline int birth_count() const;

    // When we have a birth we update the count for this BirhPLace.
    inline void RecordBirth();

    // When a birthplace is changed (updated), we need to decrement the counter
    // for the old instance.
    inline void ForgetBirth();

    // Hack to quickly reset all counts to zero.
    inline void Clear();

private:
    // The number of births on this thread for our location_.
    int birth_count_;

    DISALLOW_COPY_AND_ASSIGN(Births);
};


// Basic info summarizing multiple destructions of a tracked object with a
// single birthplace (fixed Location).  Used both on specific threads, and also
// in snapshots when integrating assembled data.
class _U2Export DeathData 
{
public:
    // Default initializer.
    DeathData();

    // When deaths have not yet taken place, and we gather data from all the
    // threads, we create DeathData stats that tally the number of births without
    // a corresponding death.
    explicit DeathData(int count);

    // Update stats for a task destruction (death) that had a Run() time of
    // |duration|, and has had a queueing delay of |queue_duration|.
    void RecordDeath(const DurationInt queue_duration,
        const DurationInt run_duration,
        int random_number);

    // Metrics accessors, used only in tests.
    int count() const;
    DurationInt run_duration_sum() const;
    DurationInt run_duration_max() const;
    DurationInt run_duration_sample() const;
    DurationInt queue_duration_sum() const;
    DurationInt queue_duration_max() const;
    DurationInt queue_duration_sample() const;

    // Construct a DictionaryValue instance containing all our stats. The caller
    // assumes ownership of the returned instance.
    base::DictionaryValue* ToValue() const;

    // Reset the max values to zero.
    void ResetMax();

    // Reset all tallies to zero. This is used as a hack on realtime data.
    void Clear();

private:
    // Members are ordered from most regularly read and updated, to least
    // frequently used.  This might help a bit with cache lines.
    // Number of runs seen (divisor for calculating averages).
    int count_;
    // Basic tallies, used to compute averages.
    DurationInt run_duration_sum_;
    DurationInt queue_duration_sum_;
    // Max values, used by local visualization routines.  These are often read,
    // but rarely updated.
    DurationInt run_duration_max_;
    DurationInt queue_duration_max_;
    // Samples, used by by crowd sourcing gatherers.  These are almost never read,
    // and rarely updated.
    DurationInt run_duration_sample_;
    DurationInt queue_duration_sample_;
};



/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2TrackedObjs_H__
