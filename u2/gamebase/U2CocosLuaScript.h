//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __U2CocosLuaScript__
#define __U2CocosLuaScript__


#include "U2Core.h"
#include "U2Mvc.h"


U2EG_NAMESPACE_BEGIN


class CocosLuaScript : public u2::LuaScript
{
public:
    CocosLuaScript(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~CocosLuaScript();
    
    virtual Script::NotificationNames listNotificationInterests(void) override;
    
    virtual void handleNotification(const Notification& notification) override;

    virtual void initialize(lua_State* mainLuaState = nullptr) override;

protected:
    void _doPredefinedViewCompCreated(const void* data);
    void _doPredefinedButtonClicked(const void* data);
    void _doPredefinedTouchesBegan(const void* data);
    void _doPredefinedTouchesMoved(const void* data);
    void _doPredefinedTouchesEnded(const void* data);
    void _doPredefinedTouchesCancelled(const void* data);
    void _doPredefinedKeyPressed(const void* data);
    void _doPredefinedKeyReleased(const void* data);
};


U2EG_NAMESPACE_END



#endif /* defined(__U2CocosLuaScript__) */
