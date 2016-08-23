#ifndef __U2FileSystemArchive_H__
#define __U2FileSystemArchive_H__

#include "U2Prerequisites.h"
#include "U2Archive.h"
#include "Threading/U2ThreadHeaders.h"
#include "U2HeaderPrefix.h"


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
class _U2Export FileSystemArchive : public Archive 
{
protected:
    /** Utility method to retrieve all files in a directory matching pattern.
    @param pattern
        File pattern.
    @param recursive
        Whether to cascade down directories.
    @param dirs
        Set to @c true if you want the directories to be listed instead of files.
    @param simpleList
        Populated if retrieving a simple list.
    @param detailList
        Populated if retrieving a detailed list.
    */
    void findFiles(const String& pattern, bool recursive, bool dirs,
        StringVector* simpleList, FileInfoList* detailList);

public:
    FileSystemArchive(const String& type, const String& name, const String& guid = BLANK);
    ~FileSystemArchive();

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

    /// Set whether filesystem enumeration will include hidden files or not.
    /// This should be called prior to declaring and/or initializing filesystem
    /// resource locations. The default is true (ignore hidden files).
    static void setIgnoreHidden(bool ignore)
    {
        msIgnoreHidden = ignore;
    }

    /// Get whether hidden files are ignored during filesystem enumeration.
    static bool getIgnoreHidden()
    {
        return msIgnoreHidden;
    }

protected:
	U2_AUTO_MUTEX;
	static bool msIgnoreHidden;
};



class FileSystemArchiveFactory : public TemplateObjectFactory < FileSystemArchive >
    , public GeneralAllocatedObject
{
public:
	virtual const String& getType() const override;
};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2FileSystemArchive_H__
