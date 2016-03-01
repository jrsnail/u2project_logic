//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "InfoMediator.h"

#include "ui/CocosGUI.h"
#include "TabInfoViewComponent.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
InfoMediator::InfoMediator(const String& type, const String& name)
    : Mediator(type, name)
{
}
//-----------------------------------------------------------------------
InfoMediator::~InfoMediator(void)
{

}
//-----------------------------------------------------------------------
void InfoMediator::onRegister(void)
{
    const TabInfoViewComponent* pConstComp = static_cast<const TabInfoViewComponent*>(getViewComponent());
    TabInfoViewComponent* pComp = const_cast<TabInfoViewComponent*>(pConstComp);

    cocos2d::ui::Button* pGoldBtn = static_cast<cocos2d::ui::Button*>(pComp->seekNodeByName("Button_coin"));
    pGoldBtn->addClickEventListener(CC_CALLBACK_0(InfoMediator::_onGoldBtnClicked, this));

    cocos2d::ui::Button* pDiamondBtn = static_cast<cocos2d::ui::Button*>(pComp->seekNodeByName("Button_diamond"));
    pDiamondBtn->addClickEventListener(CC_CALLBACK_0(InfoMediator::_onDiamondBtnClicked, this));

    cocos2d::ui::Button* pLifeBtn = static_cast<cocos2d::ui::Button*>(pComp->seekNodeByName("Button_life"));
    pLifeBtn->addClickEventListener(CC_CALLBACK_0(InfoMediator::_onLifeBtnClicked, this));

    cocos2d::ui::Button* pLvBtn = static_cast<cocos2d::ui::Button*>(pComp->seekNodeByName("Button_level"));
    pLvBtn->addClickEventListener(CC_CALLBACK_0(InfoMediator::_onLvBtnClicked, this));
}
//-----------------------------------------------------------------------
void InfoMediator::onRemove(void)
{

}
//-----------------------------------------------------------------------
Mediator::NotificationNames InfoMediator::listNotificationInterests(void) const
{
	return std::list<std::string>();
}
//-----------------------------------------------------------------------
void InfoMediator::handleNotification(const Notification& notification)
{
    const u2::String& szNtfName = notification.getName();
    const void* pData = notification.getData();
    
    {
        assert(0);
    }
}
//-----------------------------------------------------------------------
void InfoMediator::_onLvBtnClicked()
{

}
//-----------------------------------------------------------------------
void InfoMediator::_onLifeBtnClicked()
{

}
//-----------------------------------------------------------------------
void InfoMediator::_onGoldBtnClicked()
{

}
//-----------------------------------------------------------------------
void InfoMediator::_onDiamondBtnClicked()
{

}