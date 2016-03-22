//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Facade__
#define __U2Facade__


#include "U2Core.h"
#include "U2Notifier.h"
#include "U2Mediator.h"
#include "U2Command.h"
#include "U2Proxy.h"
#include "U2Model.h"
#include "U2Controller.h"
#include "U2View.h"


U2EG_NAMESPACE_BEGIN


class Model;
class View;
class Controller;


/**
* A base Multiton <code>Facade</code> implementation.
*
* @see Core/Model.hpp PureMVC::Core::Model
* @see Core/View.hpp PureMVC::Core::View
* @see Core/Controller.hpp PureMVC::Core::Controller
*/
class Facade : public Object, public Notifier
{
private:
	Facade(Facade const&);
	Facade& operator=(Facade const&);

public:
	/**
	* Constructor.
	*
	* <P>
	* This <code>Facade</code> implementation is a Multiton,
	* so you should not call the constructor
	* directly, but instead call the static Factory method,
	* passing the unique key for this instance
	* <code>Facade.getInstance( multitonKey )</code>
	*
	* @throws Error Error if instance for this Multiton key has already been constructed
	*
	*/
    explicit Facade(const String& type, const String& name);

	/**
	* Initialize the Multiton <code>Facade</code> instance.
	*
	* <P>
	* Called automatically by the constructor. Override in your
	* subclass to doany subclass specific initializations. Be
	* sure to call <code>super.initializeFacade()</code>, though.</P>
	*/
	virtual void initializeFacade(void);

protected:
	/**
	* Initialize the <code>Controller</code>.
	*
	* <P>
	* Called by the <code>initializeFacade</code> method.
	* Override this method in your subclass of <code>Facade</code>
	* if one or both of the following are true:
	* <UL>
	* <LI> You wish to initialize a different <code>Controller</code>.</LI>
	* <LI> You have <code>Commands</code> to register with the <code>Controller</code> at startup. </LI>
	* </UL>
	* If you don't want to initialize a different <code>Controller</code>,
	* call <code>super.initializeController()</code> at the beginning of your
	* method, then register <code>Command</code>s.
	* </P>
	*/
	virtual void initializeController(void);

	template <typename T>
	void initializeController(const String& name);

	/**
	* Initialize the <code>Model</code>.
	*
	* <P>
	* Called by the <code>initializeFacade</code> method.
	* Override this method in your subclass of <code>Facade</code>
	* if one or both of the following are true:
	* <UL>
	* <LI> You wish to initialize a different <code>Model</code>.</LI>
	* <LI> You have <code>Proxy</code>s to register with the Model that donot
	* retrieve a reference to the Facade at construction time.</LI>
	* </UL>
	* If you don't want to initialize a different <code>Model</code>,
	* call <code>super.initializeModel()</code> at the beginning of your
	* method, then register <code>Proxy</code>s.
	* <P>
	* Note: This method is <i>rarely</i> overridden; in practice you are more
	* likely to use a <code>Command</code> to create and register <code>Proxy</code>s
	* with the <code>Model</code>, since <code>Proxy</code>s with mutable data will likely
	* need to send <code>INotification</code>s and thus will likely want to fetch a reference to
	* the <code>Facade</code> during their construction.
	* </P>
	*/
	virtual void initializeModel(void);

	template <typename T>
	void initializeModel(const String& name);

	/**
	* Initialize the <code>View</code>.
	*
	* <P>
	* Called by the <code>initializeFacade</code> method.
	* Override this method in your subclass of <code>Facade</code>
	* if one or both of the following are true:
	* <UL>
	* <LI> You wish to initialize a different <code>View</code>.</LI>
	* <LI> You have <code>Observers</code> to register with the <code>View</code></LI>
	* </UL>
	* If you don't want to initialize a different <code>View</code>,
	* call <code>super.initializeView()</code> at the beginning of your
	* method, then register <code>Mediator</code> instances.
	* <P>
	* Note: This method is <i>rarely</i> overridden; in practice you are more
	* likely to use a <code>Command</code> to create and register <code>Mediator</code>s
	* with the <code>View</code>, since <code>Mediator</code> instances will need to send
	* <code>INotification</code>s and thus will likely want to fetch a reference
	* to the <code>Facade</code> during their construction.
	* </P>
	*/
	virtual void initializeView(void);

	template <typename T>
	void initializeView(const String& name);

	template <typename T>
	static Facade* createFacade(const String& name);

public:
	/**
	* Register an <code>Command</code> with the <code>Controller</code> by Notification name.
	*
	* @param notification_name the name of the <code>INotification</code> to associate the <code>Command</code> with
	* @param command a reference to the instance of the <code>Command</code>
	*/
	virtual void registerCommand(const String& notification_name, Command* command);

	/**
	* Remove a previously registered <code>Command</code> to <code>INotification</code> mapping from the Controller.
	*
	* @param notification_name the name of the <code>INotification</code> to remove the <code>Command</code> mapping for
	* @return the <code>Command</code> that was removed from the <code>Controller</code>
	*/
	virtual Command* removeCommand(const String& notification_name);

	/**
	* Retrieve an <code>Command</code> instance from the Controller.
	*
	* @param notification_name the notification of the <code>INotification</code>
	* @return the <code>Command</code> instance previously registered with the given <code>notification</code>.
	*/
	virtual Command const& retrieveCommand(const String& notification_name) const;

	/**
	* Retrieve an <code>Command</code> instance from the Controller.
	*
	* @param notification_name the name of the <code>INotification</code>
	* @return the <code>Command</code> instance previously registered with the given <code>notification</code>.
	*/
	virtual Command& retrieveCommand(const String& notification_name);

	/**
	* Check if a Command is registered for a given Notification
	*
	* @param notification_name
	* @return whether a Command is currently registered for the given <code>notificationName</code>.
	*/
	virtual bool hasCommand(const String& notification_name) const;

	/**
	* Register an <code>Proxy</code> with the <code>Model</code> by name.
	*
	* @param proxy the name of the <code>Proxy</code>.
	* @param proxy the <code>Proxy</code> instance to be registered with the <code>Model</code>.
	*/
	virtual void registerProxy(Proxy* proxy);

	/**
	* Retrieve an <code>Proxy</code> from the <code>Model</code> by name.
	*
	* @param proxy_name the name of the proxy to be retrieved.
	* @return the <code>Proxy</code> instance previously registered with the given <code>proxyName</code>.
	*/
	virtual Proxy const& retrieveProxy(const String& proxy_name) const;

	/**
	* Retrieve an <code>Proxy</code> from the <code>Model</code> by name.
	*
	* @param proxy_name the name of the proxy to be retrieved.
	* @return the <code>Proxy</code> instance previously registered with the given <code>proxyName</code>.
	*/
	virtual Proxy& retrieveProxy(const String& proxy_name);

    template <typename T>
    const T& retrieveProxy(const String& proxy_name) const;

    template <typename T>
    T& retrieveProxy(const String& proxy_name);

	/**
	* Remove an <code>Proxy</code> from the <code>Model</code> by name.
	*
	* @param proxy_name the <code>Proxy</code> to remove from the <code>Model</code>.
	* @return the <code>Proxy</code> that was removed from the <code>Model</code>
	*/
	virtual Proxy* removeProxy(const String& proxy_name);

	/**
	* Check if a Proxy is registered
	*
	* @param proxy_name
	* @return whether a Proxy is currently registered with the given <code>proxyName</code>.
	*/
	virtual bool hasProxy(const String& proxy_name) const;

	/**
	* Register a <code>Mediator</code> with the <code>View</code>.
	*
	* @param mediator a reference to the <code>Mediator</code>
	*/
	virtual void registerMediator(Mediator* mediator);

	/**
	* Retrieve an <code>Mediator</code> from the <code>View</code>.
	*
	* @param mediator_name
	* @return the <code>Mediator</code> previously registered with the given <code>mediatorName</code>.
	*/
	virtual Mediator const& retrieveMediator(const String& mediator_name) const;

	/**
	* Retrieve an <code>Mediator</code> from the <code>View</code>.
	*
	* @param mediator_name
	* @return the <code>Mediator</code> previously registered with the given <code>mediatorName</code>.
	*/
	virtual Mediator& retrieveMediator(const String& mediator_name);

	/**
	* Remove an <code>Mediator</code> from the <code>View</code>.
	*
	* @param mediator_name name of the <code>Mediator</code> to be removed.
	* @return the <code>Mediator</code> that was removed from the <code>View</code>
	*/
	virtual Mediator* removeMediator(const String& mediator_name);

	/**
	* Check if a Mediator is registered or not
	*
	* @param mediator_name
	* @return whether a Mediator is registered with the given <code>mediatorName</code>.
	*/
	virtual bool hasMediator(const String& mediator_name) const;

	/**
	* Create and send an <code>INotification</code>.
	*
	* <P>
	* Keeps us from having to construct new notification
	* instances in our implementation code.
	* @param notification_name the name of the notification to send
	* @param body the body of the notification (optional)
	* @param type the type of the notification (optional)
	*/
    virtual void sendNotification(const String& notification_name, const void* data = nullptr);

	/**
	* Notify <code>Observer</code>s.
	* <P>
	* This method is left public mostly for backward
	* compatibility, and to allow you to send custom
	* notification classes using the facade.</P>
	*<P>
	* Usually you should just call sendNotification
	* and pass the parameters, never having to
	* construct the notification yourself.</P>
	*
	* @param notification the <code>INotification</code> to have the <code>View</code> notify <code>Observers</code> of.
	*/
    virtual void notifyObservers(const Notification& notification);

	/**
	* Check if a Core is registered or not
	*
	* @param key the multiton key for the Core in question
	* @return whether a Core is registered with the given <code>key</code>.
	*/
	static bool hasCore(const String& name);

	/**
	* Remove a Core.
	* <P>
	* Remove the Model, View, Controller and Facade
	* instances for the given key.</P>
	*
	* @param key of the Core to remove
	*/
    static void removeCore(const String& name);

	/**
	* Broadcast notification for all Facade.
	*
	* @param notification the <code>INotification</code> to have the <code>View</code> notify <code>Observers</code> of.
	*/
    static void broadcastNotification(const Notification& notification);

	/**
	* Broadcast notification for all Facade.
	*
	* @param notification_name the name of the notification to send
	* @param body the body of the notification (optional)
	* @param type the type of the notification (optional)
	*/
    static void broadcastNotification(const String& notification_name, const void* data = nullptr);

	/**
	* Virtual destructor.
	*/
	virtual ~Facade(void);

protected:
	// References to Model, View and Controller
	Controller* _controller;
	Model* _model;
	View* _view;
};





class FacadeManager : public SimpleObjectManager<Facade>, public Singleton < FacadeManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	FacadeManager();

public:
    /** Default destructor.
    */
    virtual ~FacadeManager();

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
    static FacadeManager& getSingleton(void);

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
    static FacadeManager* getSingletonPtr(void);
};



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <typename T>
void Facade::initializeController(const String& name)
{
	_controller = ControllerManager::getSingleton().retrieveObjectByName(name);
	if (_controller == nullptr)
	{
		_controller = ControllerManager::getSingleton().createObject(GET_OBJECT_TYPE(T), name);
		_controller->initializeController();
	}
}
//-----------------------------------------------------------------------
template <typename T>
void Facade::initializeModel(const String& name)
{
	_model = ModelManager::getSingleton().retrieveObjectByName(name);
	if (_model == nullptr)
	{
		_model = ModelManager::getSingleton().createObject(GET_OBJECT_TYPE(T), name);
		_model->initializeModel();
	}
}
//-----------------------------------------------------------------------
template <typename T>
void Facade::initializeView(const String& name)
{
	_view = ViewManager::getSingleton().retrieveObjectByName(name);
	if (_view == nullptr)
	{
		_view = ViewManager::getSingleton().createObject(GET_OBJECT_TYPE(T), name);
		_view->initializeView();
	}
}
//-----------------------------------------------------------------------
template <typename T>
Facade* Facade::createFacade(const String& name)
{
	CREATE_FACTORY(T);
	Facade* pFacade = FacadeManager::getSingleton().createObject(GET_OBJECT_TYPE(T), name);
	pFacade->initializeFacade();
	return pFacade;
}
//-----------------------------------------------------------------------
template <typename T>
const T& Facade::retrieveProxy(const String& proxy_name) const
{
	return dynamic_cast<const T&>(retrieveProxy(proxy_name));
}
//-----------------------------------------------------------------------
template <typename T>
T& Facade::retrieveProxy(const String& proxy_name)
{
	return dynamic_cast<T&>(retrieveProxy(proxy_name));
}


U2EG_NAMESPACE_END

#endif /* defined(__U2Facade__) */
