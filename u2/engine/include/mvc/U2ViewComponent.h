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


U2EG_NAMESPACE_BEGIN


class ViewComponent : public Object
{
public:
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
        virtual void onCommonStateChanged(ViewComponent* viewComp, const String& objName, const String& msg) = 0;
    };

public:
    /**
    * Constructor.
    */
    ViewComponent(const String& type, const String& name);

    /**
    * Virtual destructor.
    */
    virtual ~ViewComponent(void);

    virtual String getUiName() const = 0;
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

    ViewCompState getViewCompState() { return m_ViewCompState; };

    void addListener(Listener* listener);
    void removeListener(Listener* listener);

protected:
    void _changeViewCompState(ViewCompState newState);
    void _emitCommonStateChange(const String& objName, const String& msg);

protected:
    ViewCompState       m_ViewCompState;

    typedef std::list<ViewComponent::Listener*>     ListenerList;
    ListenerList        m_listeners;
};


class ViewComponentManager : public TypedObjectManager<ViewComponent>, public Singleton < ViewComponentManager >
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
    static ViewComponentManager& getSingleton(void);

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
