//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2Notification.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
Notification::Notification(const String& name, const void* data)
    : m_szName(name)
    , m_pData(data)
{ 
}
//-----------------------------------------------------------------------
Notification::~Notification(void)
{
}
//-----------------------------------------------------------------------
inline String const& Notification::getName(void) const
{
    return m_szName;
}
//-----------------------------------------------------------------------
inline void Notification::setData(const void* data)
{
    m_pData = data;
}
//-----------------------------------------------------------------------
inline const void* Notification::getData(void) const
{
    return m_pData;
}
//-----------------------------------------------------------------------
inline String Notification::toString(void) const
{
    String msg;
    toString(msg);
    return msg;
}
//-----------------------------------------------------------------------
void Notification::toString(String& arg) const
{
    arg = "Notification Name: ";
    arg += getName();
    arg += "\nBody: ";
    arg += typeid(m_pData).name();
}
