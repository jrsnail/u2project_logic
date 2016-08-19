//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Proxy__
#define __U2Proxy__


#include "U2Core.h"
#include "U2Notifier.h"


U2EG_NAMESPACE_BEGIN

/**
* A base <code>IProxy</code> implementation.
*
* <P>
* In PureMVC, <code>Proxy</code> classes are used to manage parts of the
* application's data model. </P>
*
* <P>
* A <code>Proxy</code> might simply manage a reference to a local data object,
* in which case interacting with it might involve setting and
* getting of its data in synchronous fashion.</P>
*
* <P>
* <code>Proxy</code> classes are also used to encapsulate the application's
* interaction with remote services to save or retrieve data, in which case,
* we adopt an asyncronous idiom; setting data (or calling a method) on the
* <code>Proxy</code> and listening for a <code>Notification</code> to be sent
* when the <code>Proxy</code> has retrieved the data from the service. </P>
*
* @see Core/Model.hpp PureMVC::Core::Model
*/
class  Proxy : public Object, public Notifier, public GeneralAllocatedObject
{
public:
    /**
    * Constructor.
    */
    explicit Proxy(const String& type, const String& name, const String& guid = BLANK, void const* data = NULL);

    /**
    * Set the data object.
    */
    virtual void setData(void const* data);

    /**
    * Get the data object.
    */
    virtual void const* getData(void) const;

    /**
    * Called by the Model when the Proxy is registered.
    */
    virtual void onRegister(void);

    /**
    * Called by the Model when the Proxy is removed.
    */
    virtual void onRemove(void);

    /**
    * Virtual destructor.
    */
    virtual ~Proxy(void);

protected:
    // the data object
    void const* _data;

};


class ProxyManager : public SimpleObjectManager<Proxy>, public Singleton < ProxyManager >
{
protected:
    /** Default constructor - should never get called by a client app.
    */
    ProxyManager();

public:
    /** Default destructor.
    */
    virtual ~ProxyManager();

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
    static ProxyManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Proxy__) */
