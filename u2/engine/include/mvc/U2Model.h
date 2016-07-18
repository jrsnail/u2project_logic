//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Model__
#define __U2Model__

#include "U2Core.h"


U2EG_NAMESPACE_BEGIN


class Proxy;


class Model : public Object
{
public:
    typedef std::list<String>                       ProxyNames;

protected:
	// Mapping of proxyNames to Proxy instances
    typedef std::map<String, Proxy*>				ProxyMap;
	ProxyMap                    m_ProxyMap;

public:
	/**
	* Constructor.
	*
	* <P>
	* This <code>Model</code> implementation is a Multiton,
	* so you should not call the constructor
	* directly, but instead call the static Multiton
	* Factory method <code>Model.getInstance( multitonKey )</code>
	*
	* @throws Error Error if instance for this Multiton key instance has already been constructed
	*
	*/
    explicit Model(const String& type, const String& name);

public:
	/**
	* Initialize the <code>Model</code> instance.
	*
	* <P>
	* Called automatically by the constructor, this
	* is your opportunity to initialize the Singleton
	* instance in your subclass without overriding the
	* constructor.</P>
	*
	* @return void
	*/
	virtual void initializeModel(void);

	/**
	* Register an <code>Proxy</code> with the <code>Model</code>.
	*
	* @param proxy an <code>Proxy</code> to be held by the <code>Model</code>.
	*/
	virtual void registerProxy(Proxy* proxy);

	/**
	* Retrieve an <code>Proxy</code> from the <code>Model</code>.
	*
	* @param proxy_name
	* @return the <code>Proxy</code> instance previously registered with the given <code>proxyName</code>.
	*/
    virtual Proxy* retrieveProxy(const String& proxy_name);

	/**
	* Check if a Proxy is registered
	*
	* @param proxy_name
	* @return whether a Proxy is currently registered with the given <code>proxyName</code>.
	*/
    virtual bool hasProxy(const String& proxy_name);

	/**
	* Remove an <code>Proxy</code> from the <code>Model</code>.
	*
	* @param proxy_name name of the <code>Proxy</code> instance to be removed.
	* @return the <code>Proxy</code> that was removed from the <code>Model</code>
	*/
	virtual Proxy* removeProxy(const String& proxy_name);

	/**
	* List all names of proxy
	*
	* @return the aggregate container of <code>proxy_name</code>.
	*/
    virtual ProxyNames listProxyNames(void) const;

	/**
	* Remove an Model instance
	*
	* @param key of Model instance to remove
	*/
    static void removeModel(String const& name);

	/**
	* Virtual destructor.
	*/
	virtual ~Model(void);
};


class ModelManager : public SimpleObjectManager<Model>, public Singleton < ModelManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	ModelManager();

public:
    /** Default destructor.
    */
    virtual ~ModelManager();

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
    static ModelManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Model__) */
