#include "U2DataPool.h"

#include "U2DataFilterStream.h"
#include "U2StreamQueue.h"
#include "U2VariableMemStream.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
DataPool::DataPool(const String& type, const String& name)
    : Object(type, name)
{
}
//-----------------------------------------------------------------------
DataPool::~DataPool()
{
}
//-----------------------------------------------------------------------
bool DataPool::saveFloatData(const String& key, float value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveDoubleData(const String& key, double value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveRealData(const String& key, u2real value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveLongData(const String& key, long value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveStringData(const String& key, const String& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveBoolData(const String& key, bool value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveInt8Data(const String& key, u2int8 value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveUint8Data(const String& key, u2uint8 value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveInt16Data(const String& key, u2int16 value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveUint16Data(const String& key, u2uint16 value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveInt32Data(const String& key, u2int32 value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveUint32Data(const String& key, u2uint32 value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveInt64Data(const String& key, u2int64 value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveUint64Data(const String& key, u2uint64 value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::saveCharVectorData(const String& key, const vector<u2char>::type& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadFloatData(const String& key, float& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadDoubleData(const String& key, double& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadRealData(const String& key, u2real& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadLongData(const String& key, long& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadStringData(const String& key, String& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadBoolData(const String& key, bool& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadInt8Data(const String& key, u2int8& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadUint8Data(const String& key, u2uint8& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadInt16Data(const String& key, u2int16& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadUint16Data(const String& key, u2uint16& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadInt32Data(const String& key, u2int32& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadUint32Data(const String& key, u2uint32& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadInt64Data(const String& key, u2int64& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadUint64Data(const String& key, u2uint64& value)
{
    return false;
}
//-----------------------------------------------------------------------
bool DataPool::loadCharVectorData(const String& key, const vector<u2char>::type& value)
{
    return false;
}
//-----------------------------------------------------------------------
void DataPool::removeData(const String& key)
{

}
//-----------------------------------------------------------------------
bool DataPool::saveMemoryFloatData(const String& key, float value)
{
    U2_LOCK_MUTEX(m_MemoryMapMutex);
    vector<u2char>::type& vec = m_MemoryMap[key];

    // clear old data first
    vec.clear();

    OutStreamQueue<DataFilterOutStream> out;
    out.push<VariableMemOutStream>("aaa", &vec);
    out.push<DataFilterOutStream>("bbb");
    out->writeFloat(value);
    out->close();
    return true;
}
//-----------------------------------------------------------------------
bool DataPool::saveMemoryVec2Data(const String& key, const cocos2d::Vec2& value)
{
    U2_LOCK_MUTEX(m_MemoryMapMutex);
    vector<u2char>::type& vec = m_MemoryMap[key];

    // clear old data first
    vec.clear();

    OutStreamQueue<DataFilterOutStream> out;
    out.push<VariableMemOutStream>("aaa", &vec);
    out.push<DataFilterOutStream>("bbb");
    out->writeFloat(value.x);
    out->writeFloat(value.y);
    out->close();
    return true;
}
//-----------------------------------------------------------------------
bool DataPool::loadMemoryFloatData(const String& key, float& value)
{
    U2_LOCK_MUTEX(m_MemoryMapMutex);
    MemoryMap::iterator it = m_MemoryMap.find(key);
    if (it == m_MemoryMap.end())
    {
        return false;
    }
    vector<u2char>::type& vec = m_MemoryMap[key];

    InStreamQueue<DataFilterInStream> in;
    in.push<VariableMemInStream>("aaa", &vec);
    in.push<DataFilterInStream>("bbb");
    value = in->readFloat();
    return true;
}
//-----------------------------------------------------------------------
bool DataPool::loadMemoryVec2Data(const String& key, cocos2d::Vec2& value)
{
    U2_LOCK_MUTEX(m_MemoryMapMutex);
    MemoryMap::iterator it = m_MemoryMap.find(key);
    if (it == m_MemoryMap.end())
    {
        return false;
    }
    vector<u2char>::type& vec = m_MemoryMap[key];

    InStreamQueue<DataFilterInStream> in;
    in.push<VariableMemInStream>("aaa", &vec);
    in.push<DataFilterInStream>("bbb");
    value.x = in->readFloat();
    value.y = in->readFloat();
    return true;
}
//-----------------------------------------------------------------------
void DataPool::removeMemoryData(const String& key)
{

}
//-----------------------------------------------------------------------
void DataPool::pushTask(const String& taskLoopName, Task* task)
{
    U2_LOCK_MUTEX(m_TaskQueueMapMutex);
    TaskQueue& queue = m_TaskQueueMap[taskLoopName];
    queue.push_back(task);
}
//-----------------------------------------------------------------------
Task* DataPool::frontTask(const String& taskLoopName)
{
    U2_LOCK_MUTEX(m_TaskQueueMapMutex);
    TaskQueueMap::iterator it = m_TaskQueueMap.find(taskLoopName);
    if (it == m_TaskQueueMap.end())
    {
        return nullptr;
    }
    else
    {
        TaskQueue& queue = m_TaskQueueMap[taskLoopName];
        return queue.front();
    }
}
//-----------------------------------------------------------------------
void DataPool::popTask(const String& taskLoopName)
{
    U2_LOCK_MUTEX(m_TaskQueueMapMutex);
    TaskQueueMap::iterator it = m_TaskQueueMap.find(taskLoopName);
    if (it == m_TaskQueueMap.end())
    {
    }
    else
    {
        TaskQueue& queue = m_TaskQueueMap[taskLoopName];
        queue.pop_front();
    }
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> DataPoolManager* Singleton<DataPoolManager>::msSingleton = 0;
DataPoolManager* DataPoolManager::getSingletonPtr(void)
{
    if (msSingleton == nullptr)
    {
        msSingleton = new DataPoolManager;
    }
    return msSingleton;
}
DataPoolManager& DataPoolManager::getSingleton(void)
{
    return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
DataPoolManager::DataPoolManager()
{
}
//-----------------------------------------------------------------------
DataPoolManager::~DataPoolManager()
{
}