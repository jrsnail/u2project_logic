//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Script__
#define __U2Script__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2Notifier.h"
#include "U2Notification.h"
#include "U2SimpleObjectManager.h"


U2EG_NAMESPACE_BEGIN



class Script : public u2::Object, public Notifier
{
public:
    typedef std::list<String>		NotificationNames;
    
public:
    Script(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~Script();

    virtual void runFile(const String& fileName) = 0;
    
    virtual int runString(const String& scriptString) = 0;

    /**
     * List the <code>INotification</code> names this
     * <code>Mediator</code> is interested in being notified of.
     *
     * @return Array the list of <code>INotification</code> names.
     */
    virtual Script::NotificationNames listNotificationInterests(void);
    
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

};


class ScriptManager : public SimpleObjectManager<Script>, public Singleton < ScriptManager >
{
protected:
    /** Default constructor - should never get called by a client app.
    */
    ScriptManager();

public:
    /** Default destructor.
    */
    virtual ~ScriptManager();

    virtual Script* createObject(const String& type, const String& name = BLANK, const String& guid = BLANK);

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
    static ScriptManager* getSingletonPtr(void);

protected:
};


U2EG_NAMESPACE_END



#endif /* defined(__U2Object__) */
