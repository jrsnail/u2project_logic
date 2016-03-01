//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Mediator__
#define __U2Mediator__


#include "U2Core.h"
#include "U2Notifier.h"
#include "U2Notification.h"
#include "U2ViewComponent.h"
#include "U2TransStep.h"


U2EG_NAMESPACE_BEGIN


class Context;


/**
* A base <code>IMediator</code> implementation.
*
* @see Core/View.hpp PureMVC::Core::View
*/
class Mediator : public Object, public Notifier, public ViewComponent::Listener
{
public:
    typedef std::list<std::string>		NotificationNames;

public:
    /**
    * Constructor.
    */
    Mediator(const String& type, const String& name);

    /**
    * Virtual destructor.
    */
    virtual ~Mediator(void);

    virtual void startup(const u2::Context* context);

    virtual void end();

    /**
    * Set the <code>IMediator</code>'s view component.
    *
    * @param view_component the view component.
    */
    virtual void setViewComponent(ViewComponent* view_component);

    /**
    * Get the <code>Mediator</code>'s view component.
    *
    * <P>
    * Additionally, an implicit getter will usually
    * be defined in the subclass that casts the view
    * object to a type</P>
    *
    * @return the view component.
    */
    virtual void const* getViewComponent(void) const;

    /**
    * List the <code>INotification</code> names this
    * <code>Mediator</code> is interested in being notified of.
    *
    * @return Array the list of <code>INotification</code> names.
    */
    virtual Mediator::NotificationNames listNotificationInterests(void) const;

    /**
    * Handle <code>INotification</code>s.
    *
    * <P>
    * Typically this will be handled in a switch statement,
    * with one 'case' entry per <code>INotification</code>
    * the <code>Mediator</code> is interested in.
    */
    virtual void handleNotification(const Notification& notification);

    /**
    * Called by the View when the Mediator is registered.
    */
    virtual void onRegister(void);

    /**
    * Called by the View when the Mediator is removed.
    */
    virtual void onRemove(void);

    virtual void onUpdate(float dt);

    virtual bool preEnd(bool backKey) { return true; };
    
protected:
    virtual void onViewCompStateChanged(ViewComponent* viewComp, ViewComponent::ViewCompState newState) override {};
    virtual void onCommonStateChanged(ViewComponent* viewComp, const String& objName, const String& msg) override {};

    virtual void _registerFrameListener() {};
    virtual void _unregisterFrameListener() {};
    

protected:
    // The view component
	ViewComponent* m_pViewComp;
	bool m_bCreator;
};



class MediatorManager : public SimpleObjectManager<Mediator>, public Singleton < MediatorManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	MediatorManager();

public:
    /** Default destructor.
    */
    virtual ~MediatorManager();

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
    static MediatorManager& getSingleton(void);

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
    static MediatorManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Mediator__) */
