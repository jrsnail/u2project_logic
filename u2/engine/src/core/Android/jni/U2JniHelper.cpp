#include "U2Prerequisites.h"
#include <jni.h>
#include <EGL/egl.h>
#include <android/api-level.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Android/U2APKFileSystemArchive.h"
#include "Android/U2APKZipArchive.h"



U2EG_NAMESPACE_USING

namespace 
{

	extern "C"
	{
		JNIEXPORT void JNICALL  Java_org_u2_android_U2JniBridge_init(JNIEnv * env, jobject obj, jobject assetManager);
		JNIEXPORT void JNICALL  Java_org_u2_android_U2JniBridge_term(JNIEnv * env, jobject obj);
	}

	void Java_org_u2_android_U2JniBridge_init(JNIEnv * env, jobject obj, jobject assetManager)
	{
		// android archive
		AAssetManager* pAssetMgr = AAssetManager_fromJava(env, assetManager);
		if (pAssetMgr)
		{
			if (!u2::FactoryManager::getSingleton().hasObjectFactory("ApkFileSystem"))
			{
				u2::ObjectFactory* pObjectFactory = new ApkFileSystemArchiveFactory(pAssetMgr);
				u2::FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
			}
			if (!u2::FactoryManager::getSingleton().hasObjectFactory("ApkZip"))
			{
				u2::ObjectFactory* pObjectFactory = new ApkZipArchiveFactory(pAssetMgr);
				u2::FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
			}
		}
	}

	void Java_org_u2_android_U2JniBridge_term(JNIEnv * env, jobject obj)
	{
	}

}