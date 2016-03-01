//
//  CRemoteMsgManager.cpp
//  myGame
//
//  Created by jiang on 13-7-23.
//
//

#include "TabInfoViewComponent.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TabInfoViewComponent::TabInfoViewComponent(const String& type, const String& name)
    : CocosViewComponent(type, name)
{
}
//-----------------------------------------------------------------------
TabInfoViewComponent::~TabInfoViewComponent(void)
{

}
//-----------------------------------------------------------------------
const String& TabInfoViewComponent::getUiName() const
{
    static String name = "HomePage.csb";
    return name;
}