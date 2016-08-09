#ifndef __U2DataPool__
#define __U2DataPool__


#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2FactoryObj.h"
#include "U2Object.h"
#include "U2SimpleObjectManager.h"
#include "cocos2d.h"


U2EG_NAMESPACE_BEGIN


class Task;


class DataPool : public Object
{
public:
    DataPool(const String& type, const String& name = BLANK, const String& guid = BLANK);
    virtual ~DataPool();

    bool saveFloatData(const String& key, float value);
    bool saveDoubleData(const String& key, double value);
    bool saveRealData(const String& key, u2real value);
    bool saveLongData(const String& key, long value);
    bool saveStringData(const String& key, const String& value);
    bool saveBoolData(const String& key, bool value);
    bool saveInt8Data(const String& key, u2int8 value);
    bool saveUint8Data(const String& key, u2uint8 value);
    bool saveInt16Data(const String& key, u2int16 value);
    bool saveUint16Data(const String& key, u2uint16 value);
    bool saveInt32Data(const String& key, u2int32 value);
    bool saveUint32Data(const String& key, u2uint32 value);
    bool saveInt64Data(const String& key, u2int64 value);
    bool saveUint64Data(const String& key, u2uint64 value);
    bool saveCharVectorData(const String& key, const vector<u2char>::type& value);
    
    

    bool loadFloatData(const String& key, float& value);
    bool loadDoubleData(const String& key, double& value);
    bool loadRealData(const String& key, u2real& value);
    bool loadLongData(const String& key, long& value);
    bool loadStringData(const String& key, String& value);
    bool loadBoolData(const String& key, bool& value);
    bool loadInt8Data(const String& key, u2int8& value);
    bool loadUint8Data(const String& key, u2uint8& value);
    bool loadInt16Data(const String& key, u2int16& value);
    bool loadUint16Data(const String& key, u2uint16& value);
    bool loadInt32Data(const String& key, u2int32& value);
    bool loadUint32Data(const String& key, u2uint32& value);
    bool loadInt64Data(const String& key, u2int64& value);
    bool loadUint64Data(const String& key, u2uint64& value);
    bool loadCharVectorData(const String& key, const vector<u2char>::type& value);
    
    void removeData(const String& key);


    bool saveMemoryFloatData(const String& key, float value);
    bool saveMemoryStringData(const String& key, const String& value);
    bool saveMemoryUint64Data(const String& key, u2uint64 value);
    bool saveMemoryVec2Data(const String& key, const cocos2d::Vec2& value);

    bool loadMemoryFloatData(const String& key, float& value);
    bool loadMemoryStringData(const String& key, String& value);
    bool loadMemoryUint64Data(const String& key, u2uint64& value);
    bool loadMemoryVec2Data(const String& key, cocos2d::Vec2& value);

    void removeMemoryData(const String& key);
    

    void pushTask(const String& taskLoopName, Task* task);
    Task* frontTask(const String& taskLoopName);
    void popTask(const String& taskLoopName);

protected:
    U2_MUTEX(m_MemoryMapMutex);
    typedef map<String, vector<u2char>::type>::type  MemoryMap;
    MemoryMap           m_MemoryMap;


    U2_MUTEX(m_TaskQueueMapMutex);
    typedef list<Task*>::type       TaskQueue;
    typedef map<String, TaskQueue>::type  TaskQueueMap;
    TaskQueueMap        m_TaskQueueMap;
};


class DataPoolManager : public Singleton < DataPoolManager >, public SimpleObjectManager<DataPool>
{
public:
    DataPoolManager();
    virtual ~DataPoolManager();

public:
    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static DataPoolManager& getSingleton(void);

    /** Override standard Singleton retrieval.
    @remarks
    Why do we do this? Well, it's because the Singleton
    implementation is in a .h file, which means it gets compiled
    into anybody who includes it. This is needed for the
    Singleton template to work, but we actually only want it
    compiled into the implementation of the class based on the
    Singleton, not all of them. If we don't change this, we get
    link errors when trying to use the Singleton-based class from
    an outside dll.
    @par
    This method just delegates to the template version anyway,
    but the implementation stays in this single compilation unit,
    preventing link errors.
    */
    static DataPoolManager* getSingletonPtr(void);

protected:
};



static DataPool* DATAPOOL(String tableName)
{
    DataPool* pDataPool = DataPoolManager::getSingleton().retrieveObjectByName(tableName);
    if (pDataPool == nullptr)
    {
        assert(0);
    }
    return pDataPool;
}



U2EG_NAMESPACE_END

#endif /* defined(__U2DataPool__) */
