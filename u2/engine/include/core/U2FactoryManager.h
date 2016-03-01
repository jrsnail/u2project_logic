#ifndef __U2FactoryManager_H__
#define __U2FactoryManager_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Singleton.h"


U2EG_NAMESPACE_BEGIN


class Object;
class ObjectFactory;


class FactoryManager : public Singleton<FactoryManager>
{
protected:
    /** Default constructor - should never get called by a client app.
    */
    FactoryManager();

public:
    /** Default destructor.
    */
    virtual ~FactoryManager();

	void addObjectFactory(ObjectFactory* factory);

	bool hasObjectFactory(const String& type) const;

	void destroyObjectFactory(const String& type);

	Object* createObject(const String& type, const String& name);

	void destroyObject(Object* obj);

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
    static FactoryManager& getSingleton(void);

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
    static FactoryManager* getSingletonPtr(void);

protected:
	typedef std::map<String, ObjectFactory*>    ObjectFactoryMap;
	/// Factories available to create archives, indexed by archive type (String identifier e.g. 'Zip')
	ObjectFactoryMap                            mObjectFactories;
};



#define CREATE_FACTORY(T)                                                           \
	if (!u2::FactoryManager::getSingleton().hasObjectFactory(GET_OBJECT_TYPE(T)))   \
	{                                                                               \
		u2::ObjectFactory* pObjectFactory = new u2::TemplateObjectFactory < T > ;   \
		u2::FactoryManager::getSingleton().addObjectFactory(pObjectFactory);        \
	}

#define DESTROY_FACTORY(T)                                                          \
	u2::FactoryManager::getSingleton().destroyObjectFactory(GET_OBJECT_TYPE(T));


U2EG_NAMESPACE_END

#endif // __U2FactoryManager_H__
