#ifndef __U2Herald__
#define __U2Herald__


#include "U2Core.h"
#include "U2PredefinedPrerequisites.h"
#include "U2Notification.h"


U2EG_NAMESPACE_BEGIN


class Script;
class Observer;


class Herald : public Object, public GeneralAllocatedObject
{
protected:
    typedef std::map<String, Script*>              ScriptMap;
    typedef std::multimap<String, Observer*>       ObserverMap;
    typedef std::list<String>                      ScriptNames;

protected:
    // Mapping of Script names to Script instances
    ScriptMap                    m_ScriptMap;
    // Mapping of Notification names to Observer lists
    ObserverMap                  m_ObserverMap;

private:
    Herald(Herald const&);
    Herald& operator=(Herald const&);

public:
    explicit Herald(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~Herald(void);

    virtual void initializeHerald(void);

    virtual void registerObserver(const String& notification_name, Observer* observer);

    virtual void notifyObservers(const Notification& notification);

    virtual void removeObserver(const String& notification_name, const Object* const target);

    virtual void registerScript(Script* mediator);

    virtual Script* retrieveScript(const String& scriptName);

    virtual Script* removeScript(const String& scriptName);

    virtual bool hasScript(const String& scriptName);

    virtual ScriptNames listScriptNames(void) const;

    static void removeHerald(const String& name);

};


class HeraldManager : public SimpleObjectManager<Herald>, public Singleton < HeraldManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	HeraldManager();

public:
	/** Default destructor.
	*/
	virtual ~HeraldManager();

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
	static HeraldManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Notification__) */
