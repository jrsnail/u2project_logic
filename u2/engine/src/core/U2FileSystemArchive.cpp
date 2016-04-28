#include "U2FileSystemArchive.h"

#include "U2Exception.h"
#include "U2StringUtil.h"
#include <sys/stat.h>
#include "U2FileStream.h"

#if U2_PLATFORM == U2_PLATFORM_LINUX || U2_PLATFORM == U2_PLATFORM_APPLE || \
    U2_PLATFORM == U2_PLATFORM_APPLE_IOS || \
    U2_PLATFORM == U2_PLATFORM_ANDROID || \
    U2_PLATFORM == U2_PLATFORM_NACL || \
    U2_PLATFORM == U2_PLATFORM_EMSCRIPTEN
#   include "U2SearchOps.h"
#   include <sys/param.h>
#   define MAX_PATH MAXPATHLEN
#endif

#if U2_PLATFORM == U2_PLATFORM_WIN32 || U2_PLATFORM == U2_PLATFORM_WINRT
#	define WIN32_LEAN_AND_MEAN
#	if !defined(NOMINMAX) && defined(_MSC_VER)
#		define NOMINMAX // required to stop windows.h messing up std::min
#	endif
#	include <windows.h>
#	include <direct.h>
#	include <io.h>
#endif


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
static bool is_reserved_dir(const char *fn)
{
	return (fn[0] == '.' && (fn[1] == 0 || (fn[1] == '.' && fn[2] == 0)));
}
//-----------------------------------------------------------------------
static bool is_absolute_path(const char* path)
{
#if U2_PLATFORM == U2_PLATFORM_WIN32 || U2_PLATFORM == U2_PLATFORM_WINRT
	if (isalpha(u2uchar(path[0])) && path[1] == ':')
		return true;
#endif
	return path[0] == '/' || path[0] == '\\';
}
//-----------------------------------------------------------------------
static String concatenate_path(const String& base, const String& name)
{
	if (base.empty() || is_absolute_path(name.c_str()))
		return name;
	else
		return base + '/' + name;
}
//-----------------------------------------------------------------------
bool FileSystemArchive::msIgnoreHidden = true;
//-----------------------------------------------------------------------
FileSystemArchive::FileSystemArchive(const String& type, const String& name)
    : Archive(type, name)
{
    
}
//-----------------------------------------------------------------------
bool FileSystemArchive::isCaseSensitive(void) const
{
#if U2_PLATFORM == U2_PLATFORM_WIN32 || U2_PLATFORM == U2_PLATFORM_WINRT
	return false;
#else
	return true;
#endif
}
//-----------------------------------------------------------------------
void FileSystemArchive::findFiles(const String& pattern, bool recursive, 
    bool dirs, StringVector* simpleList, FileInfoList* detailList)
{
    intptr_t lHandle, res;
    struct _finddata_t tagData;

    // pattern can contain a directory name, separate it from mask
    size_t pos1 = pattern.rfind ('/');
    size_t pos2 = pattern.rfind ('\\');
    if (pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
        pos1 = pos2;
    String directory;
    if (pos1 != pattern.npos)
        directory = pattern.substr (0, pos1 + 1);

    String full_pattern = concatenate_path(m_szName, pattern);

    lHandle = _findfirst(full_pattern.c_str(), &tagData);
    res = 0;
    while (lHandle != -1 && res != -1)
    {
        if ((dirs == ((tagData.attrib & _A_SUBDIR) != 0)) &&
            ( !msIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0 ) &&
            (!dirs || !is_reserved_dir (tagData.name)))
        {
            if (simpleList)
            {
                simpleList->push_back(directory + tagData.name);
            }
            else if (detailList)
            {
                FileInfo fi;
                fi.archive = this;
                fi.filename = directory + tagData.name;
                fi.basename = tagData.name;
                fi.path = directory;
                fi.compressedSize = tagData.size;
                fi.uncompressedSize = tagData.size;
                detailList->push_back(fi);
            }
        }
        res = _findnext( lHandle, &tagData );
    }
    // Close if we found any files
    if(lHandle != -1)
        _findclose(lHandle);

    // Now find directories
    if (recursive)
    {
        String base_dir = m_szName;
        if (!directory.empty ())
        {
            base_dir = concatenate_path(m_szName, directory);
            // Remove the last '/'
            base_dir.erase (base_dir.length () - 1);
        }
        base_dir.append ("/*");

        // Remove directory name from pattern
        String mask ("/");
        if (pos1 != pattern.npos)
            mask.append (pattern.substr (pos1 + 1));
        else
            mask.append (pattern);

        lHandle = _findfirst(base_dir.c_str (), &tagData);
        res = 0;
        while (lHandle != -1 && res != -1)
        {
            if ((tagData.attrib & _A_SUBDIR) &&
                ( !msIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0 ) &&
                !is_reserved_dir (tagData.name))
            {
                // recurse
                base_dir = directory;
                base_dir.append (tagData.name).append (mask);
                findFiles(base_dir, recursive, dirs, simpleList, detailList);
            }
            res = _findnext( lHandle, &tagData );
        }
        // Close if we found any files
        if(lHandle != -1)
            _findclose(lHandle);
    }
}
//-----------------------------------------------------------------------
FileSystemArchive::~FileSystemArchive()
{
    unload();
}
//-----------------------------------------------------------------------
void FileSystemArchive::load()
{
    // nothing to do here
}
//-----------------------------------------------------------------------
void FileSystemArchive::unload()
{
    // nothing to see here, move along
}
//-----------------------------------------------------------------------
InStreamPtr FileSystemArchive::openForRead(const String& filename)
{
    String full_path = concatenate_path(m_szName, filename);

    // Use filesystem to determine size 
    // (quicker than streaming to the end and back)
    struct stat tagStat;
    int ret = stat(full_path.c_str(), &tagStat);
    assert(ret == 0 && "Problem getting file size" );
    (void)ret;  // Silence warning

    // Always open in binary mode
    std::ios::openmode mode = std::ios::in | std::ios::binary;
    std::ifstream* roStream = U2_NEW_T(std::ifstream, MEMCATEGORY_GENERAL)();
    roStream->open(full_path.c_str(), mode);

    // Should check ensure open succeeded, in case fail for some reason.
    if (roStream->fail())
    {
        U2_DELETE_T(roStream, basic_ifstream, MEMCATEGORY_GENERAL);
        U2_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
            "Cannot open file: " + filename,
            "FileSystemArchive::open");
    }

    // Construct return stream, tell it to delete on destroy
    FileInStream* stream = U2_NEW FileInStream(BLANK, roStream, (size_t)tagStat.st_size);
    
    return InStreamPtr(stream, deletePtr<FileInStream>);
}
//---------------------------------------------------------------------
OutStreamPtr FileSystemArchive::openForWrite(const String& filename, bool append)
{
	String full_path = concatenate_path(m_szName, filename);

	// Use filesystem to determine size 
	// (quicker than streaming to the end and back)
	struct stat tagStat;
	int ret = stat(full_path.c_str(), &tagStat);
	assert(ret == 0 && "Problem getting file size");
	(void)ret;  // Silence warning

	if (isReadOnly())
	{
		U2_EXCEPT(Exception::ERR_INVALIDPARAMS,
			"Cannot open a file in read-write mode in a read-only archive",
			"FileSystemArchive::open");
	}

	// Always open in binary mode
	std::ios::openmode mode = std::ios::out | std::ios::binary;
	if (append)
	{
		mode |= std::ios::app;
	}
	std::ofstream* rwStream = U2_NEW_T(std::ofstream, MEMCATEGORY_GENERAL)();
	rwStream->open(full_path.c_str(), mode);


	// Should check ensure open succeeded, in case fail for some reason.
	if (rwStream->fail())
	{
		U2_DELETE_T(rwStream, basic_ofstream, MEMCATEGORY_GENERAL);
		U2_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
			"Cannot open file: " + filename,
			"FileSystemArchive::open");
	}

	/// Construct return stream, tell it to delete on destroy
	FileOutStream* stream = U2_NEW FileOutStream(filename, rwStream);

	return OutStreamPtr(stream, deletePtr<FileOutStream>);
}
//---------------------------------------------------------------------
OutStreamPtr FileSystemArchive::create(const String& filename)
{
    if (isReadOnly())
    {
        U2_EXCEPT(Exception::ERR_INVALIDPARAMS, 
            "Cannot create a file in a read-only archive", 
            "FileSystemArchive::remove");
    }

    String full_path = concatenate_path(m_szName, filename);

    // Always open in binary mode
    // Also, always include reading
    std::ios::openmode mode = std::ios::out | std::ios::binary;
    std::ofstream* rwStream = U2_NEW_T(std::ofstream, MEMCATEGORY_GENERAL)();
    rwStream->open(full_path.c_str(), mode);

    // Should check ensure open succeeded, in case fail for some reason.
    if (rwStream->fail())
    {
        U2_DELETE_T(rwStream, basic_ofstream, MEMCATEGORY_GENERAL);
        U2_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
            "Cannot open file: " + filename,
            "FileSystemArchive::create");
    }

    /// Construct return stream, tell it to delete on destroy
	FileOutStream* stream = U2_NEW FileOutStream(filename, rwStream);

	return OutStreamPtr(stream, deletePtr<FileOutStream>);
}
//---------------------------------------------------------------------
void FileSystemArchive::remove(const String& filename)
{
    if (isReadOnly())
    {
        U2_EXCEPT(Exception::ERR_INVALIDPARAMS, 
            "Cannot remove a file from a read-only archive", 
            "FileSystemArchive::remove");
    }
    String full_path = concatenate_path(m_szName, filename);
    ::remove(full_path.c_str());
}
//-----------------------------------------------------------------------
StringVectorPtr FileSystemArchive::list(bool recursive, bool dirs)
{
    // directory change requires locking due to saved returns
    // Note that we have to tell the SharedPtr to use U2_DELETE_T not U2_DELETE by passing category
    StringVectorPtr ret(U2_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), deleteTPtr<StringVector>);

    findFiles("*", recursive, dirs, ret.get(), 0);

    return ret;
}
//-----------------------------------------------------------------------
FileInfoListPtr FileSystemArchive::listFileInfo(bool recursive, bool dirs)
{
    // Note that we have to tell the SharedPtr to use U2_DELETE_T not U2_DELETE by passing category
    FileInfoListPtr ret(U2_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), deleteTPtr<FileInfoList>);

    findFiles("*", recursive, dirs, 0, ret.get());

    return ret;
}
//-----------------------------------------------------------------------
StringVectorPtr FileSystemArchive::find(const String& pattern,
                                        bool recursive, bool dirs)
{
    // Note that we have to tell the SharedPtr to use U2_DELETE_T not U2_DELETE by passing category
    StringVectorPtr ret(U2_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), deleteTPtr<StringVector>);

    findFiles(pattern, recursive, dirs, ret.get(), 0);

    return ret;

}
//-----------------------------------------------------------------------
FileInfoListPtr FileSystemArchive::findFileInfo(const String& pattern, 
    bool recursive, bool dirs)
{
    // Note that we have to tell the SharedPtr to use U2_DELETE_T not U2_DELETE by passing category
    FileInfoListPtr ret(U2_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), deleteTPtr<FileInfoList>);

    findFiles(pattern, recursive, dirs, 0, ret.get());

    return ret;
}
//-----------------------------------------------------------------------
bool FileSystemArchive::exists(const String& filename)
{
    String full_path = concatenate_path(m_szName, filename);

    struct stat tagStat;
    bool ret = (stat(full_path.c_str(), &tagStat) == 0);

    // stat will return true if the filename is absolute, but we need to check
    // the file is actually in this archive
    if (ret && is_absolute_path(filename.c_str()))
    {
        // only valid if full path starts with our base
#if U2_PLATFORM == U2_PLATFORM_WIN32 || U2_PLATFORM == U2_PLATFORM_WINRT
        // case insensitive on windows
        String lowerCaseName = m_szName;
        StringUtil::toLowerCase(lowerCaseName);
        ret = StringUtil::startsWith(full_path, lowerCaseName, true);
#else
        // case sensitive
        ret = StringUtil::startsWith(full_path, m_szName, false);
#endif
    }

    return ret;
}
//---------------------------------------------------------------------
time_t FileSystemArchive::getModifiedTime(const String& filename)
{
    String full_path = concatenate_path(m_szName, filename);

    struct stat tagStat;
    bool ret = (stat(full_path.c_str(), &tagStat) == 0);

    if (ret)
    {
        return tagStat.st_mtime;
    }
    else
    {
        return 0;
    }

}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
const String& FileSystemArchiveFactory::getType() const
{
	static String name = "FileSystem";
	return name;
}