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
#include "U2Timer.h"
#include "U2Version.h"
#include "U2Stream.h"
#include "U2StringStream.h"
#include "U2VariableMemStream.h"
#include "U2MemoryStream.h"
#include "U2FileStream.h"
#include "U2FileHandleStream.h"
#include "U2ZipStream.h"
#include "U2PipedStream.h"
#include "U2FilterStream.h"
#include "U2DataFilterStream.h"
#include "U2StreamQueue.h"
#include "U2Archive.h"
#include "U2FileSystemArchive.h"
#include "U2ZipArchive.h"
#if U2_PLATFORM == U2_PLATFORM_ANDROID
#	include <android/asset_manager.h>
#	include <android/asset_manager_jni.h>
#	include "U2ApkFileSystemArchive.h"
#	include "U2ApkZipArchive.h"
#	include "U2AndroidLogListener.h"
#endif
#if U2_PLATFORM == U2_PLATFORM_APPLE_IOS
#	include "iosUtils.h"
#endif
#if U2_PLATFORM == U2_PLATFORM_APPLE
#	include "macUtils.h"
#endif
#include "U2DataPool.h"
#include "U2Component.h"
#include "U2GameObject.h"
#include "U2System.h"
#include "U2XmlSerialize.h"
#include "U2WebSocketClient.h"
#include "U2WebSocketClientImpl.h"
#include "U2SocketTask.h"
#include "U2UrlUtils.h"
#include "U2HttpClient.h"
#include "U2HttpClientImpl.h"
#include "U2LogicClient.h"
#include "U2Root.h"
#include "U2Snapshot.h"
#include "U2PredefinedPrerequisites.h"



extern "C"
{
    void initFactroy();
}


#endif
