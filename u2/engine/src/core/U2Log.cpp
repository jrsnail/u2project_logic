#include "U2Log.h"

#include <iomanip>

#if U2_PLATFORM == U2_PLATFORM_WIN32 || U2_PLATFORM == U2_PLATFORM_WINRT
#   include <windows.h>
#endif



U2EG_NAMESPACE_USING


    
//-----------------------------------------------------------------------
Log::Log( const String& name, bool debuggerOuput, bool suppressFile ) 
	: mLogLevel(LL_NORMAL)
	, mDebugOut(debuggerOuput)
	, mSuppressFile(suppressFile)
	, mTimeStamp(true)
	, mLogName(name)
{
    if (!mSuppressFile)
    {
        mLog.open(name.c_str());
    }
}
//-----------------------------------------------------------------------
Log::~Log()
{
    U2_LOCK_AUTO_MUTEX;
    if (!mSuppressFile)
    {
        mLog.close();
    }
}
//-----------------------------------------------------------------------
void Log::logMessage( const String& message, LogMessageLevel lml, bool maskDebug )
{
    U2_LOCK_AUTO_MUTEX;
    if ((mLogLevel + lml) >= U2_LOG_THRESHOLD)
    {
        bool skipThisMessage = false;
        for( mtLogListener::iterator i = mListeners.begin(); i != mListeners.end(); ++i )
            (*i)->messageLogged( message, lml, maskDebug, mLogName, skipThisMessage);
            
        if (!skipThisMessage)
        {
            if (mDebugOut && !maskDebug)
            {
#if (U2_PLATFORM == U2_PLATFORM_WIN32 || U2_PLATFORM == U2_PLATFORM_WINRT) && U2_DEBUG_MODE
#	if U2_WCHAR_T_STRINGS
                OutputDebugStringW(message.c_str());
                OutputDebugStringW(L"\n");
#	else
                OutputDebugStringA(message.c_str());
                OutputDebugStringA("\n");
#	endif
#endif
                if (lml == LML_CRITICAL)
                    std::cerr << message << std::endl;
                else
                    std::cout << message << std::endl;
            }

            // Write time into log
            if (!mSuppressFile)
            {
                if (mTimeStamp)
                {
                    struct tm *pTime;
                    time_t ctTime; time(&ctTime);
                    pTime = localtime( &ctTime );
                    mLog << std::setw(2) << std::setfill('0') << pTime->tm_hour
                        << ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
                        << ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec
                        << ": ";
                }
                mLog << message << std::endl;

                // Flush stcmdream to ensure it is written (incase of a crash, we need log to be up to date)
                mLog.flush();
            }
        }
    }
}
//-----------------------------------------------------------------------
void Log::setTimeStampEnabled(bool timeStamp)
{
    U2_LOCK_AUTO_MUTEX;
    mTimeStamp = timeStamp;
}
//-----------------------------------------------------------------------
void Log::setDebugOutputEnabled(bool debugOutput)
{
    U2_LOCK_AUTO_MUTEX;
    mDebugOut = debugOutput;
}
//-----------------------------------------------------------------------
void Log::setLogDetail(LoggingLevel ll)
{
    U2_LOCK_AUTO_MUTEX;
    mLogLevel = ll;
}
//-----------------------------------------------------------------------
void Log::addListener(LogListener* listener)
{
    U2_LOCK_AUTO_MUTEX;
    mListeners.push_back(listener);
}
//-----------------------------------------------------------------------
void Log::removeListener(LogListener* listener)
{
    U2_LOCK_AUTO_MUTEX;
    mListeners.erase(std::find(mListeners.begin(), mListeners.end(), listener));
}
//---------------------------------------------------------------------
Log::Stream Log::stream(LogMessageLevel lml, bool maskDebug) 
{
    return Stream(this, lml, maskDebug);
}