#include "U2LogManager.h"

#include "U2Exception.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
template<> LogManager* Singleton<LogManager>::msSingleton = 0;
LogManager* LogManager::getSingletonPtr(void)
{
    return msSingleton;
}
LogManager& LogManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
//-----------------------------------------------------------------------
LogManager::LogManager()
{
    mDefaultLog = NULL;
}
//-----------------------------------------------------------------------
LogManager::~LogManager()
{
    U2_LOCK_AUTO_MUTEX;
    // Destroy all logs
    LogList::iterator i;
    for (i = mLogs.begin(); i != mLogs.end(); ++i)
    {
        U2_DELETE i->second;
    }
}
//-----------------------------------------------------------------------
Log* LogManager::createLog( const String& name, bool defaultLog, bool debuggerOutput, 
    bool suppressFileOutput)
{
    U2_LOCK_AUTO_MUTEX;

    Log* newLog = U2_NEW Log(name, debuggerOutput, suppressFileOutput);

    if( !mDefaultLog || defaultLog )
    {
        mDefaultLog = newLog;
    }

    mLogs.insert( LogList::value_type( name, newLog ) );

    return newLog;
}
//-----------------------------------------------------------------------
Log* LogManager::getDefaultLog()
{
    U2_LOCK_AUTO_MUTEX;
    return mDefaultLog;
}
//-----------------------------------------------------------------------
Log* LogManager::setDefaultLog(Log* newLog)
{
    U2_LOCK_AUTO_MUTEX;
    Log* oldLog = mDefaultLog;
    mDefaultLog = newLog;
    return oldLog;
}
//-----------------------------------------------------------------------
Log* LogManager::getLog( const String& name)
{
    U2_LOCK_AUTO_MUTEX;
    LogList::iterator i = mLogs.find(name);
    if (i != mLogs.end())
        return i->second;
    else
        U2_EXCEPT(Exception::ERR_INVALIDPARAMS, "Log not found. ", "LogManager::getLog");
}
//-----------------------------------------------------------------------
void LogManager::destroyLog(const String& name)
{
    LogList::iterator i = mLogs.find(name);
    if (i != mLogs.end())
    {
        if (mDefaultLog == i->second)
        {
            mDefaultLog = 0;
        }
        U2_DELETE i->second;
        mLogs.erase(i);
    }

    // Set another default log if this one removed
    if (!mDefaultLog && !mLogs.empty())
    {
        mDefaultLog = mLogs.begin()->second;
    }
}
//-----------------------------------------------------------------------
void LogManager::destroyLog(Log* log)
{
    if(!log)
        U2_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot destroy a null log.", "LogManager::destroyLog");

    destroyLog(log->getName());
}
//-----------------------------------------------------------------------
void LogManager::logMessage( const String& message, LogMessageLevel lml, bool maskDebug)
{
    U2_LOCK_AUTO_MUTEX;
    if (mDefaultLog)
    {
        mDefaultLog->logMessage(message, lml, maskDebug);
    }
}
//-----------------------------------------------------------------------
void LogManager::setLogDetail(LoggingLevel ll)
{
    U2_LOCK_AUTO_MUTEX;
    if (mDefaultLog)
    {
        mDefaultLog->setLogDetail(ll);
    }
}
//---------------------------------------------------------------------
Log::Stream LogManager::stream(LogMessageLevel lml, bool maskDebug)
{
    U2_LOCK_AUTO_MUTEX;
    if (mDefaultLog)
        return mDefaultLog->stream(lml, maskDebug);
    else
        U2_EXCEPT(Exception::ERR_INVALIDPARAMS, "Default log not found. ", "LogManager::stream");
}