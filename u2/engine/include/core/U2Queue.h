#ifndef __U2Queue_H__
#define __U2Queue_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "threading/U2ThreadHeaders.h"


U2EG_NAMESPACE_BEGIN


template <class T> 
class Queue
{
public:
    /** Default constructor.
    */
	Queue();

    /** Default destructor.
    */
    virtual ~Queue();

	T* front();
	T* back();
	void pushBack(T* v);
	void pushFront(T* v);
	T* popFront();
	T* popBack();
	bool exist(T& v);
	bool exist(T* v);
	size_t size();
	void clear();

protected:
	U2_AUTO_MUTEX;
	list<T*>::type			m_queue;
};



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <class T>
Queue<T>::Queue()
{
}
//-----------------------------------------------------------------------
template <class T>
Queue<T>::~Queue()
{
}
//-----------------------------------------------------------------------
template <class T>
T* Queue<T>::front()
{
	U2_LOCK_AUTO_MUTEX;
	return m_queue.front();
}
//-----------------------------------------------------------------------
template <class T>
T* Queue<T>::back()
{
	U2_LOCK_AUTO_MUTEX;
	return m_queue.back();
}
//-----------------------------------------------------------------------
template <class T>
T* Queue<T>::pushBack(T* v)
{
	U2_LOCK_AUTO_MUTEX;
	return m_queue.push_back(v);
}
//-----------------------------------------------------------------------
template <class T>
T* Queue<T>::pushFront(T* v)
{
	U2_LOCK_AUTO_MUTEX;
	return m_queue.push_front(v);
}
//-----------------------------------------------------------------------
template <class T>
T* Queue<T>::popFront()
{
	U2_LOCK_AUTO_MUTEX;
	return m_queue.pop_front();
}
//-----------------------------------------------------------------------
template <class T>
T* Queue<T>::popBack()
{
	U2_LOCK_AUTO_MUTEX;
	return m_queue.pop_back();
}
//-----------------------------------------------------------------------
template <class T>
bool Queue<T>::exist(T& v)
{
	U2_LOCK_AUTO_MUTEX;
	list<T*>::iterator it = std::find(m_queue.begin(), m_queue.end(), &v);
	return it != m_queue.end();
}
//-----------------------------------------------------------------------
template <class T>
bool Queue<T>::exist(T* v)
{
	U2_LOCK_AUTO_MUTEX;
	list<T*>::iterator it = std::find(m_queue.begin(), m_queue.end(), v);
	return it != m_queue.end();
}
//-----------------------------------------------------------------------
template <class T>
size_t Queue<T>::size()
{
	U2_LOCK_AUTO_MUTEX;
	return m_queue.size();
}
//-----------------------------------------------------------------------
template <class T>
void Queue<T>::clear()
{
	U2_LOCK_AUTO_MUTEX;
	return m_queue.clear();
}


U2EG_NAMESPACE_END


#endif // __U2TypedObjectManager_H__
