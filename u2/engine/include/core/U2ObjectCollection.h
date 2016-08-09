#ifndef __U2ObjectCollection_H__
#define __U2ObjectCollection_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2IteratorWrapper.h"
#include "U2FactoryManager.h"
#include "U2Singleton.h"
#include "U2Object.h"


U2EG_NAMESPACE_BEGIN




class ObjectCollection : public Singleton < ObjectCollection >
{
protected:
	// <guid, u2::Object*>
    typedef std::map<String, u2::Object*>       ObjectMap;

public:
    /** Default constructor - should never get called by a client app.
    */
    ObjectCollection();

    /** Default destructor.
    */
    virtual ~ObjectCollection();

    typedef MapIterator<ObjectMap>          ObjectMapIterator;
	typedef ConstMapIterator<ObjectMap>     ConstObjectMapIterator;

	u2::Object* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

	void destoryObject(u2::Object* obj);

    ObjectCollection::ObjectMapIterator retrieveAllObjects()
	{
		ObjectMap::iterator bit = mObjects.begin();
		ObjectMap::iterator eit = mObjects.end();
		return ObjectMapIterator(bit, eit);
	}

	u2::Object* retrieveObjectByGuid(const String& guid);

    void renameObjectAsGuid(const String& oldGuid, const String& newGuid);
    
    
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
    static ObjectCollection& getSingleton(void);
    
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
    static ObjectCollection* getSingletonPtr(void);


protected:
	ObjectMap					mObjects;
};



U2EG_NAMESPACE_END

#endif // __U2ObjectCollection_H__
