#ifndef __U2NameGenerator_H__
#define __U2NameGenerator_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Singleton.h"
#include "U2ThreadHeaders.h"


U2EG_NAMESPACE_BEGIN


/// Utility class to generate a sequentially numbered series of names
class NameGenerator
{
protected:
    String                  mPrefix;
    uint64_t                mNext;
    U2_AUTO_MUTEX;

public:
    NameGenerator(const NameGenerator& rhs)
        : mPrefix(rhs.mPrefix), mNext(rhs.mNext) {}

    NameGenerator(const String& prefix) : mPrefix(prefix), mNext(1) {}

    /// Generate a new name
    String generate()
    {
        U2_LOCK_AUTO_MUTEX;
        u2::StringStream s;
        s << mPrefix << mNext++;
        return s.str();
    }

    /// Reset the internal counter
    void reset()
    {
        U2_LOCK_AUTO_MUTEX;
        mNext = 1ULL;
    }

    /// Manually set the internal counter (use caution)
    void setNext(uint64_t val)
    {
        U2_LOCK_AUTO_MUTEX;
        mNext = val;
    }

    /// Get the internal counter
    uint64_t getNext()
    {
        // lock even on get because 64-bit may not be atomic read
        U2_LOCK_AUTO_MUTEX;
        return mNext;
    }
};



class NameGeneratorManager : public Singleton<NameGeneratorManager>
{
protected:
    /** Default constructor - should never get called by a client app.
    */
    NameGeneratorManager();

public:
    /** Default destructor.
    */
    virtual ~NameGeneratorManager();

    void registerNameGenerator(const String& prefix);
    void unregisterNameGenerator(const String& prefix);

    String generator(const String& prefix);

    NameGenerator* getNameGenerator(const String& prefix) const;

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static NameGeneratorManager& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static NameGeneratorManager* getSingletonPtr(void);

protected:
    /// Currently loaded archives
    typedef std::map<String, NameGenerator*>    NameGeneratorMap;
    NameGeneratorMap                            m_NameGenerators;
};


U2EG_NAMESPACE_END

#endif // __U2NameGenerator_H__
