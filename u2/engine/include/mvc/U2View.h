//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2View__
#define __U2View__


#include "U2Core.h"
#include "U2Notification.h"


U2EG_NAMESPACE_BEGIN


class ViewComponent;
class Observer;


/**
* A Multiton <code>View</code> implementation.
*
* <P>
* In PureMVC, the <code>View</code> class assumes these responsibilities:
* <UL>
* <LI>Maintain a cache of <code>Mediator</code> instances.</LI>
* <LI>Provide methods for registering, retrieving, and removing <code>IMediators</code>.</LI>
* <LI>Notifiying <code>IMediators</code> when they are registered or removed.</LI>
* <LI>Managing the observer lists for each <code>INotification</code> in the application.</LI>
* <LI>Providing a method for attaching <code>IObservers</code> to an <code>INotification</code>'s observer list.</LI>
* <LI>Providing a method for broadcasting an <code>INotification</code>.</LI>
* <LI>Notifying the <code>IObservers</code> of a given <code>INotification</code> when it broadcast.</LI>
* </UL>
*
* @see Patterns/Mediator/Mediator.hpp PureMVC::Patterns::Mediator
* @see Patterns/Observer/Observer.hpp PureMVC::Patterns::Observer
* @see Patterns/Observer/Notification.hpp PureMVC::Patterns::Notification
*/
class View : public Object
{
protected:
    typedef std::map<String, ViewComponent*>       ViewCompMap;
    typedef std::multimap<String, Observer*>       ObserverMap;
    typedef std::list<String>                      ViewCompNames;

protected:
    // Mapping of Mediator names to Mediator instances
    ViewCompMap                 m_ViewCompMap;
    // Mapping of Notification names to Observer lists
    ObserverMap                 m_ObserverMap;

private:
    View(View const&);
    View& operator=(View const&);

public:
    /**
    * Constructor.
    *
    * <P>
    * This <code>View</code> implementation is a Multiton,
    * so you should not call the constructor
    * directly, but instead call the static Multiton
    * Factory method <code>View::getInstance(key)</code>
    *
    * @throws Error Error if instance for this Multiton key has already been constructed
    *
    */
    explicit View(const String& type, const String& name);

public:
    /**
    * Initialize the Singleton View instance.
    *
    * <P>
    * Called automatically by the constructor, this
    * is your opportunity to initialize the Singleton
    * instance in your subclass without overriding the
    * constructor.</P>
    *
    * @return void
    */
    virtual void initializeView(void);

    /**
    * Register an <code>IObserver</code> to be notified
    * of <code>INotifications</code> with a given name.
    *
    * @param notification_name the name of the <code>INotifications</code> to notify this <code>IObserver</code> of
    * @param observer the <code>IObserver</code> to register
    */
    virtual void registerObserver(const String& notification_name, Observer* observer);

    /**
    * Notify the <code>IObservers</code> for a particular <code>INotification</code>.
    *
    * <P>
    * All previously attached <code>IObservers</code> for this <code>INotification</code>'s
    * list are notified and are passed a reference to the <code>INotification</code> in
    * the order in which they were registered.</P>
    *
    * @param notification the <code>INotification</code> to notify <code>IObservers</code> of.
    */
    virtual void notifyObservers(Notification const& notification);

    /**
    * Remove the observer for a given notifyContext from an observer list for a given Notification name.
    * <P>
    * @param notification_name which observer list to remove from
    * @param notify_context remove the observer with this object as its notify_context
    */
    virtual void removeObserver(const String& notification_name, const Object* const target);

    /**
    * Register an <code>Mediator</code> instance with the <code>View</code>.
    *
    * <P>
    * Registers the <code>Mediator</code> so that it can be retrieved by name,
    * and further interrogates the <code>Mediator</code> for its
    * <code>INotification</code> interests.</P>
    * <P>
    * If the <code>Mediator</code> returns any <code>INotification</code>
    * names to be notified about, an <code>Observer</code> is created encapsulating
    * the <code>Mediator</code> instance's <code>handleNotification</code> method
    * and registering it as an <code>Observer</code> for all <code>INotifications</code> the
    * <code>Mediator</code> is interested in.</p>
    *
    * @param mediator a reference to the <code>Mediator</code> instance
    */
    virtual void registerViewComp(ViewComponent* mediator);

    /**
    * Retrieve an <code>Mediator</code> from the <code>View</code>.
    *
    * @param mediator_name the name of the <code>Mediator</code> instance to retrieve.
    * @return the <code>Mediator</code> instance previously registered with the given <code>mediator_name</code>.
    */
    virtual ViewComponent* retrieveViewComp(const String& viewCompName);

    /**
    * Remove an <code>Mediator</code> from the <code>View</code>.
    *
    * @param mediator_name name of the <code>Mediator</code> instance to be removed.
    * @return the <code>Mediator</code> that was removed from the <code>View</code>
    */
    virtual ViewComponent* removeViewComp(const String& viewCompName);

    /**
    * Check if a Mediator is registered or not
    *
    * @param mediator_name
    * @return whether a Mediator is registered with the given <code>mediator_name</code>.
    */
    virtual bool hasViewComp(const String& viewCompName);

    /**
    * List all names of mediator
    *
    * @return the aggregate container of <code>mediator_name</code>.
    */
    virtual ViewCompNames listViewCompNames(void) const;

    /**
    * Remove an View instance
    *
    * @param key of View instance to remove
    */
    static void removeView(const String& name);

    /**
    * Virtual destructor.
    */
    virtual ~View(void);
};


class ViewManager : public SimpleObjectManager<View>, public Singleton < ViewManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	ViewManager();

public:
	/** Default destructor.
	*/
	virtual ~ViewManager();

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
	static ViewManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Notification__) */
