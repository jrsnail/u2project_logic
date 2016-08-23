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
	return msSingleton;
}
ArchiveManager& ArchiveManager::getSingleton(void)
{
	return (*getSingletonPtr());
}
//-----------------------------------------------------------------------
ArchiveManager::ArchiveManager()
{
}
//-----------------------------------------------------------------------
ArchiveManager::~ArchiveManager()
{
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
Archive* ArchiveManager::createObject(const String& type, const String& name, const String& guid)
{
	return SimpleObjectManager<Archive>::createObject(type, name, guid);
}
//-----------------------------------------------------------------------
void ArchiveManager::destoryObject(Archive* obj)
{
	assert(obj != nullptr);
	assert(obj == SimpleObjectManager<Archive>::retrieveObjectByGuid(obj->getGuid()));
	obj->unload();
	SimpleObjectManager<Archive>::destoryObject(obj);
}