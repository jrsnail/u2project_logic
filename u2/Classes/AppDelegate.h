#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include "network/WebSocket.h" 
#include "U2GameBase.h"


U2EG_NAMESPACE_USING


namespace u2
{
    class FrameListenerCollection;
    class LogManager;
    class ComponentManager;
    class GameObjectManager;
    class Root;
}


class GameScene;


/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application , public cocos2d::network::WebSocket::Delegate
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

protected:
    virtual void onOpen(cocos2d::network::WebSocket* ws);
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data);
    virtual void onClose(cocos2d::network::WebSocket* ws);
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error);

protected:
    u2::Root*                       m_pRoot;
	u2::FrameListenerCollection*	m_pFrameListenerCollection;
	u2::LogManager*					m_pLogManager;
    u2::ComponentManager*		    m_pComponentManager;
    u2::GameObjectManager*          m_pGameObjectManager;
    GameScene*                      m_pGameScene;
};

#endif // _APP_DELEGATE_H_

