#include "Android/U2AndroidLogListener.h"
#include <android/log.h>

#define LOG_TAG   "u2"
#define LOGI(msg) ((void)__android_log_write(ANDROID_LOG_INFO, LOG_TAG, msg))
#define LOGE(msg) ((void)__android_log_write(ANDROID_LOG_ERROR, LOG_TAG, msg))

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
