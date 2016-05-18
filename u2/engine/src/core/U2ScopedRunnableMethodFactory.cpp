#include "U2Archive.h"

#include "U2Exception.h"
#include "U2FactoryManager.h"
#include "U2FileSystemArchive.h"
#include "U2ZipArchive.h"
// #if U2_PLATFORM == U2_PLATFORM_ANDROID
// #	include "U2ApkFileSystemArchive.h"
// #	include "U2ApkZipArchive.h"
// #end

U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
OutStreamPtr Archive::create(const String&)
{
    U2_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, 
                "This archive does not support creation of files.", 
                "Archive::create");
}
//---------------------------------------------------------------------
void Archive::remove(const String&)
{
    U2_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, 
                "This archive does not support removal of files.", 
                "Archive::remove");
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
template<> ArchiveManager* Singleton<ArchiveManager>::msSingleton = 0;
ArchiveManager* ArchiveManager::getSingletonPtr(void)
{
	if (msSingleton == nullptr)
	{
		msSingleton = new ArchiveManager;
	}
	return msSingleton;
}
ArchiveManager& ArchiveManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ArchiveManager::ArchiveManager()
{
	if (!u2::FactoryManager::getSingleton().hasObjectFactory("FileSystem"))
	{
		u2::ObjectFactory* pObjectFactory = new FileSystemArchiveFactory;
		u2::FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
	}
	if (!u2::FactoryManager::getSingleton().hasObjectFactory("Zip"))
	{
		u2::ObjectFactory* pObjectFactory = new ZipArchiveFactory;
		u2::FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
	}
	if (!u2::FactoryManager::getSingleton().hasObjectFactory("EmbeddedZip"))
	{
		u2::ObjectFactory* pObjectFactory = new EmbeddedZipArchiveFactory;
		u2::FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
	}

// #if U2_PLATFORM == U2_PLATFORM_ANDROID
// 	if (!u2::FactoryManager::getSingleton().hasObjectFactory("ApkFileSystem"))
// 	{
// 		u2::ObjectFactory* pObjectFactory = new ApkFileSystemArchiveFactory;
// 		u2::FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
// 	}
// 	if (!u2::FactoryManager::getSingleton().hasObjectFactory("ApkZip"))
// 	{
// 		u2::ObjectFactory* pObjectFactory = new ApkZipArchiveFactory;
// 		u2::FactoryManager::getSingleton().addObjectFactory(pObjectFactory);
// 	}
// #endif
}
//-----------------------------------------------------------------------
ArchiveManager::~ArchiveManager()
{
	DESTROY_FACTORY(FileSystemArchive);
	DESTROY_FACTORY(ZipArchive);
	DESTROY_FACTORY("EmbeddedZip");
}
//-----------------------------------------------------------------------
Archive* ArchiveManager::createObject(const String& type, const String& name, bool readOnly)
{
	Archive* pObj = createObject(type, name);
	if (pObj != nullptr)
	{
		pObj->initialize(readOnly);
		pObj->load();
	}
	return pObj;
}
//-----------------------------------------------------------------------
Archive* ArchiveManager::createObject(const String& type, const String& name)
{
	return SimpleObjectManager<Archive>::createObject(type, name);
}
//-----------------------------------------------------------------------
void ArchiveManager::destoryObject(Archive* obj)
{
	assert(obj != nullptr);
	assert(obj == SimpleObjectManager<Archive>::retrieveObjectByGuid(obj->getGuid()));
	obj->unload();
	SimpleObjectManager<Archive>::destoryObject(obj);
}