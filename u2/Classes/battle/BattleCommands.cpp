#include "BattleCommands.h"

#include "application/AppPrerequisites.h"



//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
Trans2BattleCommand::Trans2BattleCommand(const std::string& type, const std::string& name)
    : SimpleCommand(type, name)
{
}
//-----------------------------------------------------------------------
Trans2BattleCommand::~Trans2BattleCommand()
{
}
//-----------------------------------------------------------------------
void Trans2BattleCommand::go(const Notification& notification)
{
    // create context tree
    u2::Context* pRoot = ContextManager::getSingleton().retrieveObjectByName(ON_Context_Root);
    u2::Context* pJoystick = pRoot->createChild(
        OT_Context, "ON_Context_Joystick"
        , getFacade().getName()
        , OT_JoystickViewComponent, "JoystickViewComponent"
        , BLANK);
    ContextProxy* pContextProxy = getFacade(ON_Facade_Predefined).retrieveProxy<ContextProxy>(ON_Proxy_Context);
    pContextProxy->pushBack(ON_ContextQueue_Scene, pJoystick, u2::ContextQueue::eTransType::TT_Overlay);
}