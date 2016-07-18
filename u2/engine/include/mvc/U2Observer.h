//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Observer__
#define __U2Observer__


#include "U2Core.h"
#include "U2Notification.h"


U2EG_NAMESPACE_BEGIN


/**
* A base <code>IObserver</code> implementation.
*
* <P>
* An <code>IObserver</code> is an object that encapsulates information
* about an interested object with a method that should
* be called when a particular <code>INotification</code> is broadcast. </P>
*
* <P>
* In PureMVC, the <code>IObserver</code> class assumes these responsibilities:
* <UL>
* <LI>Encapsulate the notification (callback) method of the interested object.</LI>
* <LI>Encapsulate the notification context (this) of the interested object.</LI>
* <LI>Provide methods for setting the notification method and context.</LI>
* <LI>Provide a method for notifying the interested object.</LI>
* </UL>
*
* @see Core/View.hpp View
* @see Patterns/IObserver/Notification.hpp Notification
*/
class Observer : public ReusableObject
{
public:
    typedef std::function<void(const Notification&)>        NotifyCallbackFun;

private:
    Observer(void);

public:
    /**
    * Constructor.
    *
    * <P>
    * The notification method on the interested object should take
    * one parameter of type <code>INotification</code></P>
    *
    * @param notify_method the notification method of the interested object
    * @param notify_context the notification context of the interested object
    */
    explicit Observer(const String& type, const String& name)
		: ReusableObject(type)
    { }

	/**
	* Virtual destructor.
	*/
	virtual ~Observer(void)
	{ }

    virtual void preRecycleByPool() override
    {
        ReusableObject::preRecycleByPool();

        mFun = nullptr;
    }

    virtual void postReuseFromPool() override
    {
        ReusableObject::postReuseFromPool();
    }

    void setCallback(NotifyCallbackFun callback, void* target)
    {
        mFun = callback;
        m_pTarget = target;
    }

    /**
    * Notify the interested object.
    *
    * @param notification the <code>INotification</code> to pass to the interested object's notification method.
    */
    virtual void notifyObserver(Notification const& notification)
    {
        if (mFun == nullptr)
            throw std::runtime_error("Notify context is null.");
		mFun(notification);
    }

    /**
    * Compare an object to the notification context.
    *
    * @param object the object to compare.
    * @return boolean indicating if the object and the notification context are the same.
    */
    virtual bool compareNotifyTarget(const void* const target) const
    {
        return m_pTarget == target;
    }


private:
	NotifyCallbackFun mFun;
    void* m_pTarget;
};


class ObserverManager : public PoolingObjectManager<Observer>, public Singleton < ObserverManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	ObserverManager();

public:
	/** Default destructor.
	*/
	virtual ~ObserverManager();

public:
    Observer* createOrReuseObserver(const String& type
        , Observer::NotifyCallbackFun callback, void* target);

protected:
    virtual Observer* createOrReuseObject(const String& type);

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
	static ObserverManager* getSingletonPtr(void);
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Observer__) */
