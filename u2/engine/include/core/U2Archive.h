#ifndef __U2Archive_H__
#define __U2Archive_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Stream.h"
#include "U2Exception.h"
#include <ctime>
#include "U2SimpleObjectManager.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class Archive;


/** \addtogroup Core
*  @{
*/
/** \addtogroup Resources
*  @{
*/
/** Information about a file/directory within the archive will be
returned using a FileInfo struct.
@see
Archive
*/
struct FileInfo 
{
    /// The archive in which the file has been found (for info when performing
    /// multi-Archive searches, note you should still open through ResourceGroupManager)
    Archive* archive;
    /// The file's fully qualified name
    String filename;
    /// Path name; separated by '/' and ending with '/'
    String path;
    /// Base filename
    String basename;
    /// Compressed size
    size_t compressedSize;
    /// Uncompressed size
    size_t uncompressedSize;
};

typedef vector<FileInfo>::type			FileInfoList;
typedef std::shared_ptr<FileInfoList>	FileInfoListPtr;

/** Archive-handling class.
@remarks
    An archive is a generic term for a container of files. This may be a
    filesystem folder, it may be a compressed archive, it may even be 
    a remote location shared on the web. This class is designed to be 
    subclassed to provide access to a range of file locations. 
@par
    Instances of this class are never constructed or even handled by end-user
    applications. They are constructed by custom ArchiveFactory classes, 
    which plugins can register new instances of using ArchiveManager. 
    End-user applications will typically use ResourceManager or 
    ResourceGroupManager to manage resources at a higher level, rather than 
    reading files directly through this class. Doing it this way allows you
    to benefit from OGRE's automatic searching of multiple file locations 
    for the resources you are looking for.
*/
class _U2Export Archive : public Object, public GeneralAllocatedObject
{
protected:
    /// Archive name
    String mName; 
    /// Archive type code
    String mType;
    /// Read-only flag
    bool mReadOnly;

public:
    /** Constructor - don't call direct, used by ArchiveFactory.
    */
    Archive(const String& type, const String& name )
        : Object(type, name), mReadOnly(true)
	{}

    /** Default destructor.
    */
    virtual ~Archive() {}

	virtual void initialize(bool readOnly) 
	{
		mReadOnly = readOnly;
	};

    /// Returns whether this archive is case sensitive in the way it matches files
    virtual bool isCaseSensitive(void) const = 0;

    /** Loads the archive.
    @remarks
        This initializes all the internal data of the class.
    @warning
        Do not call this function directly, it is meant to be used
        only by the ArchiveManager class.
    */
    virtual void load() = 0;

    /** Unloads the archive.
    @warning
        Do not call this function directly, it is meant to be used
        only by the ArchiveManager class.
    */
    virtual void unload() = 0;

    /** Reports whether this Archive is read-only, or whether the contents
        can be updated. 
    */
    virtual bool isReadOnly() const { return mReadOnly; }

    /** Open a stream on a given file. 
    @note
        There is no equivalent 'close' method; the returned stream
        controls the lifecycle of this file operation.
    @param filename The fully qualified name of the file
    @param readOnly Whether to open the file in read-only mode or not (note, 
        if the archive is read-only then this cannot be set to false)
    @return A shared pointer to a DataStream which can be used to 
        read / write the file. If the file is not present, returns a null
        shared pointer.
    */
    virtual InStreamPtr openForRead(const String& filename) = 0;

	virtual OutStreamPtr openForWrite(const String& filename, bool append = true) = 0;

    /** Create a new file (or overwrite one already there). 
    @note If the archive is read-only then this method will fail.
    @param filename The fully qualified name of the file
    @return A shared pointer to a DataStream which can be used to 
    read / write the file. 
    */
    virtual OutStreamPtr create(const String& filename);

    /** Delete a named file.
    @remarks Not possible on read-only archives
    @param filename The fully qualified name of the file
    */
    virtual void remove(const String& filename);

    /** List all file names in the archive.
    @note
        This method only returns filenames, you can also retrieve other
        information using listFileInfo.
    @param recursive Whether all paths of the archive are searched (if the 
        archive has a concept of that)
    @param dirs Set to true if you want the directories to be listed
        instead of files
    @return A list of filenames matching the criteria, all are fully qualified
    */
    virtual StringVectorPtr list(bool recursive = true, bool dirs = false) = 0;
        
    /** List all files in the archive with accompanying information.
    @param recursive Whether all paths of the archive are searched (if the 
        archive has a concept of that)
    @param dirs Set to true if you want the directories to be listed
        instead of files
    @return A list of structures detailing quite a lot of information about
        all the files in the archive.
    */
    virtual FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false) = 0;

    /** Find all file or directory names matching a given pattern
        in this archive.
    @note
        This method only returns filenames, you can also retrieve other
        information using findFileInfo.
    @param pattern The pattern to search for; wildcards (*) are allowed
    @param recursive Whether all paths of the archive are searched (if the 
        archive has a concept of that)
    @param dirs Set to true if you want the directories to be listed
        instead of files
    @return A list of filenames matching the criteria, all are fully qualified
    */
    virtual StringVectorPtr find(const String& pattern, bool recursive = true,
        bool dirs = false) = 0;

    /** Find out if the named file exists (note: fully qualified filename required) */
    virtual bool exists(const String& filename) = 0; 

    /** Retrieve the modification time of a given file */
    virtual time_t getModifiedTime(const String& filename) = 0; 


    /** Find all files or directories matching a given pattern in this
        archive and get some detailed information about them.
    @param pattern The pattern to search for; wildcards (*) are allowed
    @param recursive Whether all paths of the archive are searched (if the 
    archive has a concept of that)
    @param dirs Set to true if you want the directories to be listed
        instead of files
    @return A list of file information structures for all files matching 
        the criteria.
    */
    virtual FileInfoListPtr findFileInfo(const String& pattern, 
        bool recursive = true, bool dirs = false) = 0;

};


class _U2Export ArchiveManager : public SimpleObjectManager<Archive>, public Singleton < ArchiveManager >
{
protected:
	/** Default constructor - should never get called by a client app.
	*/
	ArchiveManager();

public:
	/** Default destructor.
	*/
	virtual ~ArchiveManager();

	Archive* createObject(const String& type, const String& name, bool readOnly);
	virtual void destoryObject(Archive* obj);

protected:
	virtual Archive* createObject(const String& type, const String& name = BLANK);

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
	static ArchiveManager& getSingleton(void);

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
	static ArchiveManager* getSingletonPtr(void);
};


/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2Archive_H__
