#ifndef __U2ValueList_H__
#define __U2ValueList_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2IteratorWrapper.h"
#include "U2FactoryManager.h"


U2EG_NAMESPACE_BEGIN


class null_type {};

template<class T1, class T2> 
class ValueList
{
public:
	typedef T1		first_value_type;
	typedef T2		second_value_type;

	T1 m_val1;
	T2 m_val2;

	ValueList(const T1& val1, const T2& val2) : m_val1(val1), m_val2(val2) {}

	template<class V1, class V2>
	ValueList(const ValueList<V1, V2>& rhs) : m_val1(rhs.m_val1), m_val2(rhs.m_val2) {}

	explicit ValueList(null_type) {}

	ValueList() {};
};

template<class T>
struct KeyHolder
{
	typedef T		value_type;
	T				m_val;
	const bool		m_avail;
	
	KeyHolder() : m_avail(false) {}

	explicit KeyHolder(const T& v) : m_val(v), m_avail(true) {}

	template<class V>
	KeyHolder(const KeyHolder<V>& rhs) : m_avail(rhs.m_avail), m_val(rhs.m_val) {}

	template<class V> bool operator<(const KeyHolder<V>& rhs) const
	{
		return m_avail && rhs.m_avail && m_val < rhs.m_val;
	}

	template<class V> bool operator!=(const KeyHolder<V>& rhs) const
	{
		return (*this) < rhs || rhs < (*this);
	}

};

template<class T>
KeyHolder<T> ByKey(const T& t)
{
	return KeyHolder<T>(t);
}

template<class T1, class T2>
inline ValueList<T1, T2> makeValueList(const T1& val1, const T2& val2)
{
	return ValueList<T1, T2>(val1, val2);
}

template<class T1, class T2>
bool operator<(const ValueList<KeyHolder<T1>, T2>& key1, const ValueList<KeyHolder<T1>, T2>& key2)
{
	if (key1.m_val1 != key2.m_val1) 
		return key1.m_val1 < key2.m_val1;
	return  key1.m_val2 < key2.m_val2;
}

static bool operator<(null_type, null_type)
{
	return false;
}

#define TYPE_KVLIST_1(t) ValueList<KeyHolder<t>, null_type>
#define TYPE_KVLIST_2(t1,t2) ValueList<KeyHolder<t1>, TYPE_KVLIST_1(t2) >
#define TYPE_KVLIST_3(t1,t2,t3) ValueList<KeyHolder<t1>, TYPE_KVLIST_2(t2,t3) >

#define VLIST_1(v1) makeValueList(v1, null_type())
#define VLIST_2(v1,v2) makeValueList((v1), VLIST_1(v2))
#define VLIST_3(v1,v2,v3) makeValueList((v1), VLIST_2(v2,v3))
#define VLIST_4(v1,v2,v3,v4) makeValueList((v1), VLIST_3(v2,v3,v4))





U2EG_NAMESPACE_END

#endif // __U2ValueList_H__
