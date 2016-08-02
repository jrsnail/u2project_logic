#ifndef __U2Core_H__
#define __U2Core_H__


#include "U2Prerequisites.h"
#include "U2Object.h"
#include "U2Singleton.h"
#include "U2FactoryObj.h"
#include "U2FactoryManager.h"
#include "U2IteratorWrapper.h"
#include "U2NameGenerator.h"
#include "U2FactoryManager.h"
#include "U2ObjectCollection.h"
#include "U2SimpleObjectManager.h"
#include "U2TypedObjectManager.h"
#include "U2PoolingObjectManager.h"

#include "U2MemoryAllocatorConfig.h"
#include "U2MemoryAllocatedObject.h"
#include "U2STLRedefined.h"
#include "U2ThreadHeaders.h"
#include "U2Log.h"
#include "U2LogManager.h"
#include "U2Exception.h"
#include "U2FrameListenerCollection.h"
#include "U2StringUtil.h"
#include "U2Version.h"
#include "U2DataPool.h"
#include "U2PredefinedComponents.h"



extern "C"
{
    void initFactroy();
}


#endif
