#ifndef __U2Command__
#define __U2Command__


#include "U2Core.h"
#include "U2Notifier.h"
#include "U2Notification.h"


U2EG_NAMESPACE_BEGIN


class Command : public Object, public Notifier
{
public:
    Command(const String& type, const String& name);
    virtual ~Command();

    virtual void go(const Notification& notification) = 0;
    virtual void back() = 0;
    virtual bool canBack() = 0;
    virtual String serialize() = 0;
    virtual void deserialize(const String& str) = 0;

};



class CommandManager : public TypedObjectManager<Command>, public Singleton < CommandManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	CommandManager();

public:
    /** Default destructor.
    */
    virtual ~CommandManager();

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
    static CommandManager& getSingleton(void);

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
    static CommandManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Command__) */
