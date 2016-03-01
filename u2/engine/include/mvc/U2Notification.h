//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2Notification__
#define __U2Notification__


#include "U2Core.h"


U2EG_NAMESPACE_BEGIN


class Notification
{
public:
    /**
    * Constructor.
    *
    * @param name name of the <code>Notification</code> instance. (required)
    * @param body the <code>Notification</code> body. (optional)
    * @param type the type of the <code>Notification</code> (optional)
    */
    Notification(const String& name, const void* data = nullptr);

    /**
    * Get the name of the <code>Notification</code> instance.
    *
    * @return the name of the <code>Notification</code> instance.
    */
    virtual String const& getName(void) const;

    /**
    * Virtual destructor.
    */
    virtual ~Notification(void);

    /**
    * Set the body of the <code>Notification</code> instance.
    */
    virtual void setData(const void* data);

    /**
    * Get the body of the <code>Notification</code> instance.
    *
    * @return the body object.
    */
    virtual const void* getData(void) const;

    /**
    * Get the string representation of the <code>Notification</code> instance.
    *
    * @return the string representation of the <code>Notification</code> instance.
    */
    virtual String toString(void) const;

    /**
    * Get the string representation of the <code>INotification</code> instance.
    */
    virtual void toString(String& arg) const;

private:
    // the body of the notification instance
    const void* m_pData;
    // the name of the notification instance
    String m_szName;
};


U2EG_NAMESPACE_END

#endif /* defined(__U2Notification__) */
