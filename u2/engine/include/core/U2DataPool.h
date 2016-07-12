//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2DataPool__
#define __U2DataPool__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2FactoryObj.h"
#include "U2Object.h"
#include "U2SimpleObjectManager.h"


U2EG_NAMESPACE_BEGIN


class DataPool : public Object
{
public:
    DataPool(const String& type, const String& name = BLANK);
    virtual ~DataPool();

protected:
};


class DataPoolManager : public Singleton < DataPoolManager >, public SimpleObjectManager<DataPool>
{
public:
    DataPoolManager();
    virtual ~DataPoolManager();

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
    static DataPoolManager& getSingleton(void);

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
    static DataPoolManager* getSingletonPtr(void);

protected:
};



U2EG_NAMESPACE_END

#endif /* defined(__U2DataPool__) */
