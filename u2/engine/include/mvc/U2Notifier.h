//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Notifier__
#define __U2Notifier__


#include "U2Core.h"


U2EG_NAMESPACE_BEGIN


class Facade;

class Notifier
{
public:
    /**
    * Constructor.
    */
    explicit Notifier();

    /**
    * Initialize this INotifier instance.
    * <P>
    * This is how a Notifier gets its multitonKey.
    * Calls to sendNotification or to access the
    * facade will fail until after this method
    * has been called.</P>
    *
    * <P>
    * Mediators, Commands or Proxies may override
    * this method in order to send notifications
    * or access the Multiton Facade instance as
    * soon as possible. They CANNOT access the facade
    * in their constructors, since this method will not
    * yet have been called.</P>
    *
    * @param key the multitonKey for this INotifier to use.
    */
    virtual void initializeNotifier(const String& facadeKey);

    /**
    * Create and send an <code>INotification</code>.
    *
    * <P>
    * Keeps us from having to construct new INotification
    * instances in our implementation code.
    * @param notification_name the name of the notiification to send.
    * @param body the body of the notification (optional).
    * @param type the type of the notification (optional).
    */
    virtual void sendNotification(const String& notification_name, const void* data = nullptr);

    /**
    * Virtual destructor.
    */
    virtual ~Notifier(void);

protected:
    // Return the Multiton Facade instance
    Facade& getFacade();

    Facade& getFacade(const String& facadeKey);

protected:
    String          m_szFacadeKey;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Notifier__) */
