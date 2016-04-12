#ifndef __U2AndroidLogListener_H__
#define __U2AndroidLogListener_H__

#include "U2Log.h"

namespace u2
{
    
    class AndroidLogListener : public u2::LogListener, public u2::LogAlloc
    {
    public:
        AndroidLogListener();
    
        virtual void messageLogged(const u2::String& message, u2::LogMessageLevel lml
			, bool maskDebug, const u2::String &logName, bool& skipThisMessage );
    };
}

#endif
