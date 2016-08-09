#ifndef __U2FactoryObj_H__
#define __U2FactoryObj_H__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"


U2EG_NAMESPACE_BEGIN


/** Abstract factory class. Does nothing by itself, but derived classes can add
	functionality.
*/
template< typename T > 
class FactoryObj
{
public:
	virtual ~FactoryObj() {}

	/** Returns the factory type.
	@return
		The factory type.
	*/
	virtual const String& getType() const = 0;

	/** Creates a new object.
	@param name Name of the object to create
	@return
		An object created by the factory. The type of the object depends on
		the factory.
	*/
    virtual T* createInstance(const String& name, const String& guid) = 0;

	/** Destroys an object which was created by this factory.
	@param ptr Pointer to the object to destroy
	*/
	virtual void destroyInstance(T* ptr) = 0;
};


U2EG_NAMESPACE_END


#endif // _FactoryObj_H__
