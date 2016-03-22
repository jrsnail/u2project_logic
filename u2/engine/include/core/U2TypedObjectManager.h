#ifndef __U2TypedObjectManager_H__
#define __U2TypedObjectManager_H__


#include "U2Prerequisites.h"
#include "U2ObjectCollection.h"


U2EG_NAMESPACE_BEGIN


template <class T> 
class TypedObjectManager : public ObjectCollection<T>
{
protected:
	void addObject(T* obj)
	{
		ObjectCollection<T>::addObject(obj);
	}

	void removeObject(T* obj)
	{
		ObjectCollection<T>::removeObject(obj);
	}
};


U2EG_NAMESPACE_END

#endif // __U2TypedObjectManager_H__
