#include "U2DataPool.h"


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
void DataPool::removeData(const String& key)
{

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