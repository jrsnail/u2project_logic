#ifndef __U2ApkFileSystemArchive_H__
#define __U2ApkFileSystemArchive_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Archive.h"
#include "Threading/U2ThreadHeaders.h"
#include <android/asset_manager.h>


U2EG_NAMESPACE_BEGIN


/** \addtogroup Core
*  @{
*/
/** \addtogroup Resources
*  @{
*/
/** Specialisation of the Archive class to allow reading of files from 
    filesystem folders / directories.
*/
class _U2Export ApkFileSystemArchive : public Archive 
{
public:
    ApkFileSystemArchive(const String& type, const String& name);
    ~ApkFileSystemArchive();

	void initializeAAsset(AAssetManager* AssetMgr);

    /// @copydoc Archive::isCaseSensitive
    virtual bool isCaseSensitive(void) const override;

    /// @copydoc Archive::load
	virtual void load() override;

    /// @copydoc Archive::unload
	virtual void unload() override;

    /// @copydoc Archive::openForRead
	virtual InStreamPtr openForRead(const String& filename) override;

	/// @copydoc Archive::openForWrite
	virtual OutStreamPtr openForWrite(const String& filename, bool append) override;

    /// @copydoc Archive::create
	virtual OutStreamPtr create(const String& filename) override;

    /// @copydoc Archive::remove
	virtual void remove(const String& filename) override;

    /// @copydoc Archive::list
	virtual StringVectorPtr list(bool recursive = true, bool dirs = false) override;

    /// @copydoc Archive::listFileInfo
	virtual FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false) override;

    /// @copydoc Archive::find
	virtual StringVectorPtr find(const String& pattern, bool recursive = true,
        bool dirs = false) override;

    /// @copydoc Archive::findFileInfo
	virtual FileInfoListPtr findFileInfo(const String& pattern, bool recursive = true,
        bool dirs = false) override;

    /// @copydoc Archive::exists
	virtual bool exists(const String& filename) override;

    /// @copydoc Archive::getModifiedTime
	virtual time_t getModifiedTime(const String& filename) override;


protected:
	U2_AUTO_MUTEX;
	AAssetManager* mAssetMgr;
	String mPathPreFix;
};



class ApkFileSystemArchiveFactory : public TemplateObjectFactory < ApkFileSystemArchive >
{
public:
	ApkFileSystemArchiveFactory(AAssetManager* AssetMgr);

	virtual Object* createInstance(const String& name) override;

protected:
	AAssetManager* mAssetMgr;
};


/** @} */
/** @} */

U2EG_NAMESPACE_END


#endif // __U2ApkFileSystemArchive_H__
