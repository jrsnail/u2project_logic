#ifndef __U2Timer_H__
#define __U2Timer_H__

#include "U2Platform.h"

//Bring in the specific platform's header file
#if U2_PLATFORM == U2_PLATFORM_WIN32 || U2_PLATFORM == U2_PLATFORM_WINRT
# include "Win/U2TimerImp.h"
#elif U2_PLATFORM == U2_PLATFORM_APPLE
# include "OSX/U2MacTimerImp.h"
#elif U2_PLATFORM == U2_PLATFORM_APPLE_IOS
# include "iOS/U2IosTimerImp.h"
#elif U2_PLATFORM == U2_PLATFORM_ANDROID
# include "Android/U2TimerImp.h"
#endif

#endif
