//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2FrameListenerCollection__
#define __U2FrameListenerCollection__


#include "U2Core.h"


U2EG_NAMESPACE_BEGIN


class FrameListenerCollection : public Singleton<FrameListenerCollection>
{
protected:
    typedef std::function<void(float)>      Func;
    typedef std::map<void*, Func>           FuncMap;
    typedef ConstMapIterator<FuncMap>       ConstFuncMapIterator;
    typedef MapIterator<FuncMap>            FuncMapIterator;

protected:
    /** Default constructor - should never get called by a client app.
    */
    FrameListenerCollection();

public:
    /** Default destructor.
    */
    virtual ~FrameListenerCollection();

    virtual void registerFrameListener() {};

    virtual void unregisterFrameListener() {};

    void addFrameListener(void* address, Func fun);

    void removeFrameListener(void* address);

protected:
    void _onUpdate(float dt);

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
    static FrameListenerCollection& getSingleton(void);

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
    static FrameListenerCollection* getSingletonPtr(void);

protected:
    FuncMap         m_funcMap;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2FrameListenerCollection__) */
