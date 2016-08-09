#ifndef __U2ZipArchive_H__
#define __U2ZipArchive_H__

#include "U2Prerequisites.h"
#include "U2Archive.h"
#include "Threading/U2ThreadHeaders.h"
#include <zzip/zzip.h>
#include <zzip/plugin.h>
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


/** \addtogroup Core
*  @{
*/
/** \addtogroup Resources
*  @{
*/
/** Specialisation of the Archive class to allow reading of files from a zip
	format source archive.
@remarks
	This archive format supports all archives compressed in the standard
	zip format, including iD pk3 files.
*/
class _U2Export ZipArchive : public Archive
{
protected:
	/// Handle any errors from zzip
	void checkZzipError(int zzipError, const String& operation) const;

public:
	ZipArchive(const String& type, const String& name, const String& guid = BLANK);

	~ZipArchive();

	void initializeZip(zzip_plugin_io_handlers* pluginIo = nullptr);

	/// @copydoc Archive::isCaseSensitive
	virtual bool isCaseSensitive(void) const  override { return false; }

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
	/// Handle to root zip file
	ZZIP_DIR*					mZzipDir;
	/// File list (since zziplib seems to only allow scanning of dir tree once)
	FileInfoList				mFileList;
	/// A pointer to file io alternative implementation 
	zzip_plugin_io_handlers*	mPluginIo;
};


class ZipArchiveFactory : public TemplateObjectFactory < ZipArchive >
{
public:
	virtual const String& getType() const override;
};


class EmbeddedZipArchiveFactory : public TemplateObjectFactory < ZipArchive >
{
public:
	EmbeddedZipArchiveFactory();

	virtual const String& getType() const override;

	virtual Object* createInstance(const String& name, const String& guid = BLANK) override;

	/** a function type to decrypt embedded zip file
	@param pos pos in file
	@param buf current buffer to decrypt
	@param len - length of buffer
	@return success
	*/
	typedef bool(*DecryptEmbeddedZipFileFunc)(size_t pos, void* buf, size_t len);

	/// Add an embedded file to the embedded file list
	static void addEmbbeddedFile(const String& name, const u2uint8 * fileData,
		size_t fileSize, DecryptEmbeddedZipFileFunc decryptFunc);

	/// Remove an embedded file to the embedded file list
	static void removeEmbbeddedFile(const String& name);

protected:
	/// A static pointer to file io alternative implementation for the embedded files
	static zzip_plugin_io_handlers* mPluginIo;
};


/** @} */
/** @} */


U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2ZipArchive_H__
