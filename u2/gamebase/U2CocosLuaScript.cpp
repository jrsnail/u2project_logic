//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "U2CocosLuaScript.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "U2FrameListenerCollection.h"
#include "U2CocosPrerequisites.h"
#include "U2AllLuaBridge.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
CocosLuaScript::CocosLuaScript(const String& type, const String& name, const String& guid)
    : LuaScript(type, name, guid)
{

}
//-----------------------------------------------------------------------
CocosLuaScript::~CocosLuaScript()
{
}
//-----------------------------------------------------------------------
void CocosLuaScript::initialize(lua_State* mainLuaState)
{
    LuaScript::initialize(mainLuaState);

    luaopen_u2lib(m_pThreadState);

    luaopen_meta_LuaTask(m_pThreadState);
    luaopen_meta_Context(m_pThreadState);

    createConstant4Lua(m_pThreadState, this);
}
//-----------------------------------------------------------------------
Script::NotificationNames CocosLuaScript::listNotificationInterests(void)
{
//     m_NotificationNames.push_back(NTF_Predefined_ViewComp_Created);
//     m_NotificationNames.push_back(NTF_Predefined_Button_Clicked);
//     m_NotificationNames.push_back(NTF_Predefined_Touches_Began);
//     m_NotificationNames.push_back(NTF_Predefined_Touches_Moved);
//     m_NotificationNames.push_back(NTF_Predefined_Touches_Ended);
//     m_NotificationNames.push_back(NTF_Predefined_Touches_Cancelled);
//     m_NotificationNames.push_back(NTF_Predefined_KeyPressed);
//     m_NotificationNames.push_back(NTF_Predefined_KeyReleased);

    return m_NotificationNames;
}
//-----------------------------------------------------------------------
void CocosLuaScript::handleNotification(const Notification& notification)
{
    const String& szNtfName = notification.getName();
    const void* pData = notification.getData();

    if (szNtfName == NTF_Predefined_ViewComp_Created)
    {
        _doPredefinedViewCompCreated(pData);
    }
    else if (szNtfName == NTF_Predefined_Button_Clicked)
    {
        _doPredefinedButtonClicked(pData);
    }
    else if (szNtfName == NTF_Predefined_Touches_Began)
    {
        _doPredefinedTouchesBegan(pData);
    }
    else if (szNtfName == NTF_Predefined_Touches_Moved)
    {
        _doPredefinedTouchesMoved(pData);
    }
    else if (szNtfName == NTF_Predefined_Touches_Ended)
    {
        _doPredefinedTouchesEnded(pData);
    }
    else if (szNtfName == NTF_Predefined_Touches_Cancelled)
    {
        _doPredefinedTouchesCancelled(pData);
    }
    else if (szNtfName == NTF_Predefined_KeyPressed)
    {
        _doPredefinedKeyPressed(pData);
    }
    else if (szNtfName == NTF_Predefined_KeyReleased)
    {
        _doPredefinedKeyReleased(pData);
    }
}
//-----------------------------------------------------------------------
void CocosLuaScript::_doPredefinedViewCompCreated(const void* data)
{
    typedef std::tuple<String, String, String> ViewCompCreatedData;
    const ViewCompCreatedData* pData = static_cast<const ViewCompCreatedData*>(data);
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::LuaTask* pTask = (u2::LuaTask*)u2::TaskManager::getSingletonPtr()->createObject(OT_C2LTask_ViewCompCreated, BLANK);
        if (pTask != nullptr)
        {
            pTask->setField("ViewCompGuid", std::get<0>(*pData));
            pTask->setField("ViewCompName", std::get<1>(*pData));
            pTask->setField("ViewCompCsb", std::get<2>(*pData));
            this->pushC2LTask(pTask);
        }
    }
}
//-----------------------------------------------------------------------
void CocosLuaScript::_doPredefinedButtonClicked(const void* data)
{
    typedef std::tuple<String, String, String, String> BtnClickedData;
    const BtnClickedData* pData = static_cast<const BtnClickedData*>(data);
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::LuaTask* pTask = (u2::LuaTask*)u2::TaskManager::getSingletonPtr()->createObject(OT_C2LTask_ButtonCliecked, BLANK);
        if (pTask != nullptr)
        {
            pTask->setField("ViewCompGuid", std::get<0>(*pData));
            pTask->setField("ViewCompName", std::get<1>(*pData));
            pTask->setField("ViewCompCsb", std::get<2>(*pData));
            pTask->setField("WidgetName", std::get<3>(*pData));
            this->pushC2LTask(pTask);
        }
    }
}
//-----------------------------------------------------------------------
void CocosLuaScript::_doPredefinedTouchesBegan(const void* data)
{
    typedef std::tuple<String, String, String, const std::vector<cocos2d::Touch*>> TouchesBeganData;
    const TouchesBeganData* pData = static_cast<const TouchesBeganData*>(data);
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::LuaTask* pTask = (u2::LuaTask*)u2::TaskManager::getSingletonPtr()->createObject(OT_C2LTask_TouchesBegan, BLANK);
        if (pTask != nullptr)
        {
            pTask->setField("ViewCompGuid", std::get<0>(*pData));
            pTask->setField("ViewCompName", std::get<1>(*pData));
            pTask->setField("ViewCompCsb", std::get<2>(*pData));

            // touches
            const std::vector<cocos2d::Touch*>& vec = std::get<3>(*pData);
            pTask->setField("TouchesCount", StringUtil::toString(vec.size()));
            size_t i = 1;
            for (std::vector<cocos2d::Touch*>::const_iterator it = vec.begin(); it != vec.end(); it++)
            {
                cocos2d::Vec2 cor = (*it)->getLocation();

                StringStream sx;
                sx << "x" << i;
                pTask->setField(sx.str(), StringUtil::toString(cor.x));

                StringStream sy;
                sy << "y" << i;
                pTask->setField(sy.str(), StringUtil::toString(cor.y));

                i++;
            }
            this->pushC2LTask(pTask);
        }
    }
}
//-----------------------------------------------------------------------
void CocosLuaScript::_doPredefinedTouchesMoved(const void* data)
{
    typedef std::tuple<String, String, String, const std::vector<cocos2d::Touch*>> TouchesMovedData;
    const TouchesMovedData* pData = static_cast<const TouchesMovedData*>(data);
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::LuaTask* pTask = (u2::LuaTask*)u2::TaskManager::getSingletonPtr()->createObject(OT_C2LTask_TouchesMoved, BLANK);
        if (pTask != nullptr)
        {
            pTask->setField("ViewCompGuid", std::get<0>(*pData));
            pTask->setField("ViewCompName", std::get<1>(*pData));
            pTask->setField("ViewCompCsb", std::get<2>(*pData));

            // touches
            const std::vector<cocos2d::Touch*>& vec = std::get<3>(*pData);
            pTask->setField("TouchesCount", StringUtil::toString(vec.size()));
            size_t i = 1;
            for (std::vector<cocos2d::Touch*>::const_iterator it = vec.begin(); it != vec.end(); it++)
            {
                cocos2d::Vec2 cor = (*it)->getLocation();

                StringStream sx;
                sx << "x" << i;
                pTask->setField(sx.str(), StringUtil::toString(cor.x));

                StringStream sy;
                sy << "y" << i;
                pTask->setField(sy.str(), StringUtil::toString(cor.y));

                i++;
            }
            this->pushC2LTask(pTask);
        }
    }
}
//-----------------------------------------------------------------------
void CocosLuaScript::_doPredefinedTouchesEnded(const void* data)
{
    typedef std::tuple<String, String, String, const std::vector<cocos2d::Touch*>> TouchesEndedData;
    const TouchesEndedData* pData = static_cast<const TouchesEndedData*>(data);
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::LuaTask* pTask = (u2::LuaTask*)u2::TaskManager::getSingletonPtr()->createObject(OT_C2LTask_TouchesEnded, BLANK);
        if (pTask != nullptr)
        {
            pTask->setField("ViewCompGuid", std::get<0>(*pData));
            pTask->setField("ViewCompName", std::get<1>(*pData));
            pTask->setField("ViewCompCsb", std::get<2>(*pData));

            // touches
            const std::vector<cocos2d::Touch*>& vec = std::get<3>(*pData);
            pTask->setField("TouchesCount", StringUtil::toString(vec.size()));
            size_t i = 1;
            for (std::vector<cocos2d::Touch*>::const_iterator it = vec.begin(); it != vec.end(); it++)
            {
                cocos2d::Vec2 cor = (*it)->getLocation();

                StringStream sx;
                sx << "x" << i;
                pTask->setField(sx.str(), StringUtil::toString(cor.x));

                StringStream sy;
                sy << "y" << i;
                pTask->setField(sy.str(), StringUtil::toString(cor.y));

                i++;
            }
            this->pushC2LTask(pTask);
        }
    }
}
//-----------------------------------------------------------------------
void CocosLuaScript::_doPredefinedTouchesCancelled(const void* data)
{
    typedef std::tuple<String, String, String, const std::vector<cocos2d::Touch*>> TouchesCancelledData;
    const TouchesCancelledData* pData = static_cast<const TouchesCancelledData*>(data);
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::LuaTask* pTask = (u2::LuaTask*)u2::TaskManager::getSingletonPtr()->createObject(OT_C2LTask_TouchesCancelled, BLANK);
        if (pTask != nullptr)
        {
            pTask->setField("ViewCompGuid", std::get<0>(*pData));
            pTask->setField("ViewCompName", std::get<1>(*pData));
            pTask->setField("ViewCompCsb", std::get<2>(*pData));

            // touches
            const std::vector<cocos2d::Touch*>& vec = std::get<3>(*pData);
            pTask->setField("TouchesCount", StringUtil::toString(vec.size()));
            size_t i = 1;
            for (std::vector<cocos2d::Touch*>::const_iterator it = vec.begin(); it != vec.end(); it++)
            {
                cocos2d::Vec2 cor = (*it)->getLocation();

                StringStream sx;
                sx << "x" << i;
                pTask->setField(sx.str(), StringUtil::toString(cor.x));

                StringStream sy;
                sy << "y" << i;
                pTask->setField(sy.str(), StringUtil::toString(cor.y));

                i++;
            }
            this->pushC2LTask(pTask);
        }
    }
}
//-----------------------------------------------------------------------
void CocosLuaScript::_doPredefinedKeyPressed(const void* data)
{
    typedef std::tuple<String, String, String, cocos2d::EventKeyboard::KeyCode> KeyPressedData;
    const KeyPressedData* pData = static_cast<const KeyPressedData*>(data);
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::LuaTask* pTask = (u2::LuaTask*)u2::TaskManager::getSingletonPtr()->createObject(OT_C2LTask_KeyPressed, BLANK);
        if (pTask != nullptr)
        {
            pTask->setField("ViewCompGuid", std::get<0>(*pData));
            pTask->setField("ViewCompName", std::get<1>(*pData));
            pTask->setField("ViewCompCsb", std::get<2>(*pData));
            pTask->setField("KeyCode", StringUtil::toString((size_t)std::get<3>(*pData)));
            this->pushC2LTask(pTask);
        }
    }
}
//-----------------------------------------------------------------------
void CocosLuaScript::_doPredefinedKeyReleased(const void* data)
{
    typedef std::tuple<String, String, String, cocos2d::EventKeyboard::KeyCode> KeyReleasedData;
    const KeyReleasedData* pData = static_cast<const KeyReleasedData*>(data);
    if (pData == nullptr)
    {
        assert(0);
    }
    else
    {
        u2::LuaTask* pTask = (u2::LuaTask*)u2::TaskManager::getSingletonPtr()->createObject(OT_C2LTask_KeyReleased, BLANK);
        if (pTask != nullptr)
        {
            pTask->setField("ViewCompGuid", std::get<0>(*pData));
            pTask->setField("ViewCompName", std::get<1>(*pData));
            pTask->setField("ViewCompCsb", std::get<2>(*pData));
            pTask->setField("KeyCode", StringUtil::toString((size_t)std::get<3>(*pData)));
            this->pushC2LTask(pTask);
        }
    }
}