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