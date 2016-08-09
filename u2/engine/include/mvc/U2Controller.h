//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Controller__
#define __U2Controller__


#include "U2Core.h"
#include "U2Notification.h"
#include "U2View.h"


U2EG_NAMESPACE_BEGIN


class Command;
class View;

class Controller : public Object
{
public:
    //typedef std::list<String>                   NotificationNames;

protected:
    // Local reference to View
    View*                       m_pView;
    // Mapping of Notification names to Command Class references
    typedef std::map<String, String>          CommandMap;
    CommandMap                  m_CommandMap;

private:
    Controller(Controller const&);
    Controller& operator=(Controller const&);

public:
    /**
    * Constructor.
    *
    * <P>
    * This <code>Controller</code> implementation is a Multiton,
    * so you should not call the constructor
    * directly, but instead call the static Factory method,
    * passing the unique key for this instance
    * <code>Controller::getInstance( key )</code>
    *
    * @throws Error Error if instance for this Multiton key has already been constructed
    *
    */
    explicit Controller(const String& type, const String& name = BLANK, const String& guid = BLANK);

public:
    /**
    * Initialize the Multiton <code>Controller</code> instance.
    *
    * <P>Called automatically by the constructor.</P>
    *
    * <P>Note that if you are using a subclass of <code>View</code>
    * in your application, you should <i>also</i> subclass <code>Controller</code>
    * and override the <code>initializeController</code> method in the
    * following way:</P>
    *
    * <code>
    *        // ensure that the Controller is talking to my IView implementation
    *        override public function initializeController(  ) : void
    *        {
    *            view = MyView.getInstance();
    *        }
    * </code>
    *
    * @return void
    */
    virtual void initializeController(void);

	template <typename T>
	void initializeView(const String& name);

    /**
    * If an <code>Command</code> has previously been registered
    * to handle a the given <code>INotification</code>, then it is executed.
    *
    * @param notification an <code>INotification</code>
    */
    virtual void executeCommand(const Notification& notification);

    /**
    * Register a particular <code>Command</code> class as the handler
    * for a particular <code>INotification</code>.
    *
    * <P>
    * If an <code>Command</code> has already been registered to
    * handle <code>INotification</code>s with this name, it is no longer
    * used, the new <code>Command</code> is used instead.</P>
    *
    * The Observer for the new Command is only created if this the
    * first time an Command has been regisered for this Notification name.
    *
    * @param notification_name the name of the <code>INotification</code>
    * @param command the command of the <code>Command</code>
    */
    virtual void registerCommand(const String& notification_name, const String& cmdType);

    /**
    * Check if a Command is registered for a given Notification
    *
    * @param notification_name
    * @return whether a Command is currently registered for the given <code>notification_name</code>.
    */
    virtual bool hasCommand(const String& notification_name);

    /**
    * Retrieve an <code>Command</code> instance from the Controller.
    *
    * @param notification_name the name of the <code>INotification</code>
    * @return the <code>Command</code> instance previously registered with the given <code>notification</code>.
    */
    virtual String retrieveCommand(const String& notification_name);

    /**
    * Remove a previously registered <code>Command</code> to <code>INotification</code> mapping.
    *
    * @param notification_name the name of the <code>INotification</code> to remove the <code>Command</code> mapping for
    * @return the <code>Command</code> that was removed from the <code>Controller</code>
    */
    virtual String removeCommand(const String& notification_name);

    /**
    * List all notification name
    *
    * @return the aggregate container of <code>notification_name</code>.
    */
    virtual std::list<String> listNotificationNames(void) const;

    /**
    * Remove an Controller instance
    *
    * @param key of Controller instance to remove
    */
    static void removeController(const String& name);

    /**
    * Virtual destructor.
    */
    virtual ~Controller(void);
};


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template <typename T>
void Controller::initializeView(const String& name)
{
    CREATE_FACTORY(T);
	m_pView = ViewManager::getSingletonPtr()->retrieveObjectByName(name);
	if (m_pView == nullptr)
	{
		m_pView = ViewManager::getSingletonPtr()->createObject(GET_OBJECT_TYPE(T), name);
		m_pView->initializeView();
	}
}


class ControllerManager : public SimpleObjectManager<Controller>, public Singleton < ControllerManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	ControllerManager();

public:
    /** Default destructor.
    */
    virtual ~ControllerManager();

    Controller* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

    Controller* retrieveObjectByName(const String& name);

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
    static ControllerManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Controller__) */
