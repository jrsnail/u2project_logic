//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2ViewComponent__
#define __U2ViewComponent__


#include "U2Core.h"
#include "U2Notifier.h"
#include "U2Notification.h"
#include "U2SimpleObjectManager.h"
#include "U2Object.h"


U2EG_NAMESPACE_BEGIN


class ViewComponent : public u2::Object, public Notifier
{
public:
    typedef std::list<String>		NotificationNames;

    enum class ViewCompState
    {
        VCS_Unloading,
        VCS_Unloaded,
        VCS_Loading,
        VCS_Loaded,
        VCS_Attaching,
        VCS_Attached,
        VCS_Detaching,
        VCS_Detached,
        VCS_Entering,
        VCS_Entered,
        VCS_Exiting,
        VCS_Exited,
    };
    //static const String VCS_Unloading;

    class Listener
    {
    public:
        virtual void onViewCompStateChanged(ViewComponent* viewComp, ViewCompState newState) = 0;
    };

public:
    ViewComponent(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~ViewComponent(void);

    inline void initialize(const String& uiName)
    {
        m_szUiName = uiName;
    }

    virtual const String& getUiName() const { return m_szUiName; };
    virtual void loadUi() = 0;
    virtual void onUiLoaded() = 0;
    virtual void unloadUi() = 0;
    virtual void onWillUiUnload() = 0;
    virtual void enter() = 0;
    virtual void onEntered() = 0;
    virtual void exit() = 0;
    virtual void onWillExit() = 0;
    virtual void attach(void* parent) = 0;
    virtual void detach(void* parent) = 0;
    virtual void* getParent() const = 0;
    virtual void* getSelf() const = 0;

    ViewCompState getViewCompState() { return m_ViewCompState; };

    void addListener(Listener* listener);
    void removeListener(Listener* listener);

    /**
    * List the <code>INotification</code> names this
    * <code>Mediator</code> is interested in being notified of.
    *
    * @return Array the list of <code>INotification</code> names.
    */
    virtual ViewComponent::NotificationNames listNotificationInterests(void) const;

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

    virtual void startup() {};

    virtual void end() {};

    virtual bool preEnd(bool backKey) { return true; };


protected:
    void _changeViewCompState(ViewCompState newState);

protected:
    ViewCompState       m_ViewCompState;

    typedef std::list<ViewComponent::Listener*>     ListenerList;
    ListenerList        m_listeners;

    String         m_szUiName;
};


class ViewComponentManager : public SimpleObjectManager<ViewComponent>, public Singleton < ViewComponentManager >
{
protected:
    /** Default constructor - should never get called by a client app.
    */
    ViewComponentManager();

public:
    /** Default destructor.
    */
    virtual ~ViewComponentManager();

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
    static ViewComponentManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Mediator__) */
