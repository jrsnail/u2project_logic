//
//  CRemoteMsgManager.h
//  myGame
//
//  Created by jiang on 13-7-23.
//
//
#ifndef __TabInfoPrerequisites__
#define __TabInfoPrerequisites__


#include "cocos2d.h"
#include "U2Prerequisites.h"
#include "U2STLRedefined.h"


U2EG_NAMESPACE_USING


extern const char* ObjectTypeTabInfoFacade;
extern const char* ObjectTypeTabInfoCreateCommand;
extern const char* ObjectTypeTabInfoDestroyCommand;
extern const char* ObjectTypeTabInfoMediator;
extern const char* ObjectTypeTabMediator;
extern const char* ObjectTypeInfoMediator;
extern const char* ObjectTypeTabInfoViewComponent;


// tabinfo
const u2::String Ntf_Tabinfo_Create = "Ntf_Tabinfo_Create";
const String Ntf_Tabinfo_Destroy = "Ntf_Tabinfo_Destroy";

// tab
const String Ntf_HomeTab_Clicked = "Ntf_HomeTab_Clicked";
const String Ntf_MusicTab_Clicked = "Ntf_MusicTab_Clicked";
const String Ntf_HallTab_Clicked = "Ntf_HallTab_Clicked";
const String Ntf_SettingTab_Clicked = "Ntf_SettingTab_Clicked";

// info
const String Ntf_LvBtn_Clicked = "Ntf_LvBtn_Clicked";
const String Ntf_GoldBtn_Clicked = "Ntf_GoldBtn_Clicked";
const String Ntf_LifeBtn_Clicked = "Ntf_LifeBtn_Clicked";
const String Ntf_DiamondBtn_Clicked = "Ntf_DiamondBtn_Clicked";

#endif /* defined(__TabInfoPrerequisites__) */
