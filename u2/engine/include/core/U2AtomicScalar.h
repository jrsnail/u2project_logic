#ifndef __U2AtomicScalar_H__
#define __U2AtomicScalar_H__

#include "U2Prerequisites.h"
#include <atomic>


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
    {
	}

    AtomicScalar () 
    {
	}

    T get (void) const
    {
        return mField.load();
    }

    void set (const T &v)
    {
        mField.store(v); 
    }   

    bool cas (const T &old, const T &nu)
    {
		T expected = old;
        return mField.compare_exchange_strong(expected, nu);
    }
            
    T operator++ (void)
    {
        return ++mField;
    }
            
    T operator-- (void)
    {
        return --mField;
    }

    T operator++ (int)
    {
        return mField++;
    }
            
    T operator-- (int)
    {
        return mField--;
    }

    T operator+=(const T &add)
    {
        return mField += add;
    }

    T operator-=(const T &sub)
    {
        return mField -= sub;
    }

private:
	AtomicScalar(const AtomicScalar& rhs) = delete;
	AtomicScalar& operator=(const AtomicScalar& rhs) = delete;
	AtomicScalar(AtomicScalar&& rhs) = delete;
	AtomicScalar& operator=(AtomicScalar&& rhs) = delete;

protected:
	std::atomic<T> mField;

};
/** @} */
/** @} */

U2EG_NAMESPACE_END


#endif

