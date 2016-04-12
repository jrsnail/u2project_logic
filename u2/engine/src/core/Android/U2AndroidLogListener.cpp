#include "Android/U2AndroidLogListener.h"
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "u2", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "u2", __VA_ARGS__))

namespace u2
{
    AndroidLogListener::AndroidLogListener()
    {
    }

    void AndroidLogListener::messageLogged(const String& message, LogMessageLevel lml
		, bool maskDebug, const String &logName, bool& skipThisMessage )
    {
        if(lml < u2::LML_CRITICAL)
        {
            LOGI(message.c_str());
        }
        else
        {
            LOGE(message.c_str());
        }
    }
}
