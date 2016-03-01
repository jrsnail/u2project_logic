//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "TabMediator.h"

#include "ui/CocosGUI.h"
#include "TabInfoViewComponent.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TabMediator::TabMediator(const String& type, const String& name)
    : Mediator(type, name)
{
}
//-----------------------------------------------------------------------
TabMediator::~TabMediator(void)
{

}
//-----------------------------------------------------------------------
void TabMediator::onRegister(void)
{
    const TabInfoViewComponent* pConstComp = static_cast<const TabInfoViewComponent*>(getViewComponent());
    TabInfoViewComponent* pComp = const_cast<TabInfoViewComponent*>(pConstComp);

    cocos2d::ui::Button* pHomeTab = static_cast<cocos2d::ui::Button*>(pComp->seekNodeByName("btn_home"));
    pHomeTab->addClickEventListener(CC_CALLBACK_0(TabMediator::_onHomeTabClicked, this));

    cocos2d::ui::Button* pMusicTab = static_cast<cocos2d::ui::Button*>(pComp->seekNodeByName("btn_music"));
    pMusicTab->addClickEventListener(CC_CALLBACK_0(TabMediator::_onMusicTabClicked, this));

    cocos2d::ui::Button* pHallTab = static_cast<cocos2d::ui::Button*>(pComp->seekNodeByName("btn_hall"));
    pHallTab->addClickEventListener(CC_CALLBACK_0(TabMediator::_onHallTabClicked, this));

    cocos2d::ui::Button* pSettingTab = static_cast<cocos2d::ui::Button*>(pComp->seekNodeByName("btn_me"));
    pSettingTab->addClickEventListener(CC_CALLBACK_0(TabMediator::_onSettingTabClicked, this));
}
//-----------------------------------------------------------------------
void TabMediator::onRemove(void)
{

}
//-----------------------------------------------------------------------
Mediator::NotificationNames TabMediator::listNotificationInterests(void) const
{
	return std::list<std::string>();
}
//-----------------------------------------------------------------------
void TabMediator::handleNotification(const Notification& notification)
{
    const u2::String& szNtfName = notification.getName();
    const void* pData = notification.getData();
    
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
void TabMediator::_onHomeTabClicked()
{

}
//-----------------------------------------------------------------------
void TabMediator::_onMusicTabClicked()
{

}
//-----------------------------------------------------------------------
void TabMediator::_onHallTabClicked()
{

}
//-----------------------------------------------------------------------
void TabMediator::_onSettingTabClicked()
{

}