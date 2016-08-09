#include "U2ZipArchive.h"

#include "U2Exception.h"
#include "U2LogManager.h"
#include "U2StringUtil.h"
#include "U2ZipStream.h"


U2EG_NAMESPACE_USING


/// Utility method to format out zzip errors
String getZzipErrorDescription(zzip_error_t zzipError)
{
	String errorMsg;
	switch (zzipError)
	{
	case ZZIP_NO_ERROR:
		break;
	case ZZIP_OUTOFMEM:
		errorMsg = "Out of memory.";
		break;
	case ZZIP_DIR_OPEN:
	case ZZIP_DIR_STAT:
	case ZZIP_DIR_SEEK:
	case ZZIP_DIR_READ:
		errorMsg = "Unable to read zip file.";
		break;
	case ZZIP_UNSUPP_COMPR:
		errorMsg = "Unsupported compression format.";
		break;
	case ZZIP_CORRUPTED:
		errorMsg = "Corrupted archive.";
		break;
	case ZZIP_DIR_TOO_SHORT:
		errorMsg = "Zip file is too short.";
		break;
	case ZZIP_DIR_EDH_MISSING:
		errorMsg = "Zip-file's central directory record missing. Is this a 7z file?";
		break;
	default:
		errorMsg = "Unknown error.";
		break;
	};

	return errorMsg;
}
//-----------------------------------------------------------------------
struct FileNameCompare : public std::binary_function<FileInfo, String, bool>
{
	bool operator()(const FileInfo& lhs, const String& filename) const
	{
		return lhs.filename == filename;
	}
};
//-----------------------------------------------------------------------
ZipArchive::ZipArchive(const String& type, const String& name, const String& guid)
	: Archive(type, name, guid)
	, mZzipDir(0)
	, mPluginIo(nullptr)
{
}
//-----------------------------------------------------------------------
ZipArchive::~ZipArchive()
{
	unload();
}
//-----------------------------------------------------------------------
void ZipArchive::initializeZip(zzip_plugin_io_handlers* pluginIo)
{
	mPluginIo = pluginIo;
}
//-----------------------------------------------------------------------
void ZipArchive::load()
{
	U2_LOCK_AUTO_MUTEX;
	if (!mZzipDir)
	{
		zzip_error_t zzipError;
		mZzipDir = zzip_dir_open_ext_io(m_szName.c_str(), &zzipError, 0, mPluginIo);
		checkZzipError(zzipError, "opening archive");

		// Cache names
		ZZIP_DIRENT zzipEntry;
		while (zzip_dir_read(mZzipDir, &zzipEntry))
		{
			FileInfo info;
			info.archive = this;
			// Get basename / path
			StringUtil::splitFilename(zzipEntry.d_name, info.basename, info.path);
			info.filename = zzipEntry.d_name;
			// Get sizes
			info.compressedSize = static_cast<size_t>(zzipEntry.d_csize);
			info.uncompressedSize = static_cast<size_t>(zzipEntry.st_size);
			// folder entries
			if (info.basename.empty())
			{
				info.filename = info.filename.substr(0, info.filename.length() - 1);
				StringUtil::splitFilename(info.filename, info.basename, info.path);
				// Set compressed size to -1 for folders; anyway nobody will check
				// the compressed size of a folder, and if he does, its useless anyway
				info.compressedSize = size_t(-1);
			}
			else
			{
				info.filename = info.basename;
			}
			mFileList.push_back(info);
		}
	}
}
//-----------------------------------------------------------------------
void ZipArchive::unload()
{
	U2_LOCK_AUTO_MUTEX;
	if (mZzipDir)
	{
		zzip_dir_close(mZzipDir);
		mZzipDir = 0;
		mFileList.clear();
	}
}
//-----------------------------------------------------------------------
InStreamPtr ZipArchive::openForRead(const String& filename)
{
	// zziplib is not threadsafe
	U2_LOCK_AUTO_MUTEX;
	String lookUpFileName = filename;

	// Format not used here (always binary)
	ZZIP_FILE* zzipFile =
		zzip_file_open(mZzipDir, lookUpFileName.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS);
	if (!zzipFile) // Try if we find the file
	{
		const FileInfoListPtr fileNfo = findFileInfo(lookUpFileName, true);
		if (fileNfo->size() == 1) // If there are more files with the same do not open anyone
		{
			FileInfo info = fileNfo->at(0);
			lookUpFileName = info.path + info.basename;
			zzipFile = zzip_file_open(mZzipDir, lookUpFileName.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS); // When an error happens here we will catch it below
		}
	}

	if (!zzipFile)
	{
		int zerr = zzip_error(mZzipDir);
		String zzDesc = getZzipErrorDescription((zzip_error_t)zerr);
		LogManager::getSingleton().logMessage(
			m_szName + " - Unable to open file " + lookUpFileName + ", error was '" + zzDesc + "'", LML_CRITICAL);

		// return null pointer
		return InStreamPtr();
	}

	// Get uncompressed size too
	ZZIP_STAT zstat;
	zzip_dir_stat(mZzipDir, lookUpFileName.c_str(), &zstat, ZZIP_CASEINSENSITIVE);

	// Construct & return stream
	return InStreamPtr(U2_NEW ZipInStream(lookUpFileName, zzipFile, static_cast<size_t>(zstat.st_size)), deletePtr<ZipInStream>);
}
//---------------------------------------------------------------------
OutStreamPtr ZipArchive::openForWrite(const String& filename, bool append)
{
	U2_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
		"Modification of zipped archives is not supported",
		"ZipArchive::openForWrite");
}
//---------------------------------------------------------------------
OutStreamPtr ZipArchive::create(const String& filename)
{
	U2_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
		"Modification of zipped archives is not supported",
		"ZipArchive::create");
}
//---------------------------------------------------------------------
void ZipArchive::remove(const String& filename)
{
	U2_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
		"Modification of zipped archives is not supported",
		"ZipArchive::remove");
}
//-----------------------------------------------------------------------
StringVectorPtr ZipArchive::list(bool recursive, bool dirs)
{
	U2_LOCK_AUTO_MUTEX;
	StringVectorPtr ret = StringVectorPtr(U2_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), deleteTPtr<StringVector>);

	FileInfoList::iterator i, iend;
	iend = mFileList.end();
	for (i = mFileList.begin(); i != iend; ++i)
		if ((dirs == (i->compressedSize == size_t(-1))) &&
			(recursive || i->path.empty()))
			ret->push_back(i->filename);

	return ret;
}
//-----------------------------------------------------------------------
FileInfoListPtr ZipArchive::listFileInfo(bool recursive, bool dirs)
{
	U2_LOCK_AUTO_MUTEX;
	FileInfoList* fil = U2_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)();
	FileInfoList::const_iterator i, iend;
	iend = mFileList.end();
	for (i = mFileList.begin(); i != iend; ++i)
		if ((dirs == (i->compressedSize == size_t(-1))) &&
			(recursive || i->path.empty()))
			fil->push_back(*i);

	return FileInfoListPtr(fil, deleteTPtr<FileInfoList>);
}
//-----------------------------------------------------------------------
StringVectorPtr ZipArchive::find(const String& pattern, bool recursive, bool dirs)
{
	U2_LOCK_AUTO_MUTEX;
	StringVectorPtr ret = StringVectorPtr(U2_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), deleteTPtr<StringVector>);
	// If pattern contains a directory name, do a full match
	bool full_match = (pattern.find('/') != String::npos) ||
		(pattern.find('\\') != String::npos);
	bool wildCard = pattern.find("*") != String::npos;

	FileInfoList::iterator i, iend;
	iend = mFileList.end();
	for (i = mFileList.begin(); i != iend; ++i)
		if ((dirs == (i->compressedSize == size_t(-1))) &&
			(recursive || full_match || wildCard))
			// Check basename matches pattern (zip is case insensitive)
			if (StringUtil::match(full_match ? i->filename : i->basename, pattern, false))
				ret->push_back(i->filename);

	return ret;
}
//-----------------------------------------------------------------------
FileInfoListPtr ZipArchive::findFileInfo(const String& pattern,
	bool recursive, bool dirs)
{
	U2_LOCK_AUTO_MUTEX;
	FileInfoListPtr ret = FileInfoListPtr(U2_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), deleteTPtr<FileInfoList>);
	// If pattern contains a directory name, do a full match
	bool full_match = (pattern.find('/') != String::npos) ||
		(pattern.find('\\') != String::npos);
	bool wildCard = pattern.find("*") != String::npos;

	FileInfoList::const_iterator i, iend;
	iend = mFileList.end();
	for (i = mFileList.begin(); i != iend; ++i)
		if ((dirs == (i->compressedSize == size_t(-1))) &&
			(recursive || full_match || wildCard))
			// Check name matches pattern (zip is case insensitive)
			if (StringUtil::match(full_match ? i->filename : i->basename, pattern, false))
				ret->push_back(*i);

	return ret;
}
//-----------------------------------------------------------------------
bool ZipArchive::exists(const String& filename)
{
	U2_LOCK_AUTO_MUTEX;
	String cleanName = filename;
	if (filename.rfind("/") != String::npos)
	{
		StringVector tokens = StringUtil::split(filename, "/");
		cleanName = tokens[tokens.size() - 1];
	}

	return std::find_if(mFileList.begin(), mFileList.end(), std::bind2nd<FileNameCompare>(FileNameCompare(), cleanName)) != mFileList.end();
}
//---------------------------------------------------------------------
time_t ZipArchive::getModifiedTime(const String& filename)
{
	// Zziplib doesn't yet support getting the modification time of individual files
	// so just check the mod time of the zip itself
	struct stat tagStat;
	bool ret = (stat(m_szName.c_str(), &tagStat) == 0);

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
void ZipArchive::checkZzipError(int zzipError, const String& operation) const
{
	if (zzipError != ZZIP_NO_ERROR)
	{
		String errorMsg = getZzipErrorDescription(static_cast<zzip_error_t>(zzipError));

		U2_EXCEPT(Exception::ERR_INTERNAL_ERROR,
			m_szName + " - error whilst " + operation + ": " + errorMsg,
			"ZipArchive::checkZzipError");
	}
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
const String& ZipArchiveFactory::getType() const
{
	static String name = "Zip";
	return name;
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//  EmbeddedZipArchiveFactory
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
/// a struct to hold embedded file data
struct EmbeddedFileData
{
	const u2uint8 * fileData;
	zzip_size_t fileSize;
	zzip_size_t curPos;
	bool isFileOpened;
	EmbeddedZipArchiveFactory::DecryptEmbeddedZipFileFunc decryptFunc;
};
//-----------------------------------------------------------------------
/// A type for a map between the file names to file index
typedef map<String, int>::type			FileNameToIndexMap;
typedef FileNameToIndexMap::iterator	FileNameToIndexMapIter;
/// A type to store the embedded files data
typedef vector<EmbeddedFileData>::type	EmbbedFileDataList;
/// A static map between the file names to file index
FileNameToIndexMap *					EmbeddedZipArchiveFactory_mFileNameToIndexMap;
/// A static list to store the embedded files data
EmbbedFileDataList *					EmbeddedZipArchiveFactory_mEmbbedFileDataList;
/// A static pointer to file io alternative implementation for the embedded files
zzip_plugin_io_handlers*				EmbeddedZipArchiveFactory::mPluginIo = nullptr;
_zzip_plugin_io							sEmbeddedZipArchiveFactory_PluginIo;
#define EMBED_IO_BAD_FILE_HANDLE (-1)
#define EMBED_IO_SUCCESS (0)
//-----------------------------------------------------------------------
/// functions for embedded zzip_plugin_io_handlers implementation 
/// The functions are here and not as static members because they 
/// use types that I don't want to define in the header like zzip_char_t,
//  zzip_ssize_t and such.
//-----------------------------------------------------------------------
// get file date by index
EmbeddedFileData & getEmbeddedFileDataByIndex(int fd)
{
	return (*EmbeddedZipArchiveFactory_mEmbbedFileDataList)[fd - 1];
}
//-----------------------------------------------------------------------
// opens the file
int EmbeddedZipArchiveFactory_open(zzip_char_t* name, int flags, ...)
{
	String nameAsString = name;
	FileNameToIndexMapIter foundIter = EmbeddedZipArchiveFactory_mFileNameToIndexMap->find(nameAsString);
	if (foundIter != EmbeddedZipArchiveFactory_mFileNameToIndexMap->end())
	{
		int fd = foundIter->second;
		EmbeddedFileData & curEmbeddedFileData = getEmbeddedFileDataByIndex(fd);
		if (curEmbeddedFileData.isFileOpened)
		{
			// file is opened - return an error handle
			return EMBED_IO_BAD_FILE_HANDLE;
		}

		curEmbeddedFileData.isFileOpened = true;
		return fd;
	}
	else
	{
		// not found - return an error handle
		return EMBED_IO_BAD_FILE_HANDLE;
	}
}
//-----------------------------------------------------------------------
// Closes a file.
// Return Value - On success, close returns 0. 
int EmbeddedZipArchiveFactory_close(int fd)
{
	if (fd == EMBED_IO_BAD_FILE_HANDLE)
	{
		// bad index - return an error
		return -1;
	}

	EmbeddedFileData & curEmbeddedFileData = getEmbeddedFileDataByIndex(fd);

	if (curEmbeddedFileData.isFileOpened == false)
	{
		// file is opened - return an error handle
		return -1;
	}
	else
	{
		// success
		curEmbeddedFileData.isFileOpened = false;
		curEmbeddedFileData.curPos = 0;
		return 0;
	}
}
//-----------------------------------------------------------------------
// reads data from the file
zzip_ssize_t EmbeddedZipArchiveFactory_read(int fd, void* buf, zzip_size_t len)
{
	if (fd == EMBED_IO_BAD_FILE_HANDLE)
	{
		// bad index - return an error size - negative
		return -1;
	}
	// get the current buffer in file;
	EmbeddedFileData & curEmbeddedFileData = getEmbeddedFileDataByIndex(fd);
	const u2uint8 * curFileData = curEmbeddedFileData.fileData;
	if (len + curEmbeddedFileData.curPos > curEmbeddedFileData.fileSize)
	{
		len = curEmbeddedFileData.fileSize - curEmbeddedFileData.curPos;
	}
	curFileData += curEmbeddedFileData.curPos;

	// copy to out buffer
	memcpy(buf, curFileData, len);

	if (curEmbeddedFileData.decryptFunc != NULL)
	{
		if (!curEmbeddedFileData.decryptFunc(curEmbeddedFileData.curPos, buf, len))
		{
			// decrypt failed - return an error size - negative
			return -1;
		}
	}

	// move the cursor to the new pos
	curEmbeddedFileData.curPos += len;

	return len;
}
//-----------------------------------------------------------------------
// Moves file pointer.
zzip_off_t EmbeddedZipArchiveFactory_seeks(int fd, zzip_off_t offset, int whence)
{
	if (fd == EMBED_IO_BAD_FILE_HANDLE)
	{
		// bad index - return an error - nonzero value.
		return -1;
	}

	zzip_size_t newPos = -1;
	// get the current buffer in file;
	EmbeddedFileData & curEmbeddedFileData = getEmbeddedFileDataByIndex(fd);
	switch (whence)
	{
	case SEEK_CUR:
		newPos = (zzip_size_t)(curEmbeddedFileData.curPos + offset);
		break;
	case SEEK_END:
		newPos = (zzip_size_t)(curEmbeddedFileData.fileSize - offset);
		break;
	case SEEK_SET:
		newPos = (zzip_size_t)offset;
		break;
	default:
		// bad whence - return an error - nonzero value.
		return -1;
		break;
	};
	if (newPos >= curEmbeddedFileData.fileSize)
	{
		// bad whence - return an error - nonzero value.
		return -1;
	}

	curEmbeddedFileData.curPos = newPos;
	return newPos;
}
//-----------------------------------------------------------------------
// returns the file size
zzip_off_t EmbeddedZipArchiveFactory_filesize(int fd)
{
	if (fd == EMBED_IO_BAD_FILE_HANDLE)
	{
		// bad index - return an error - nonzero value.
		return -1;
	}
	// get the current buffer in file;
	EmbeddedFileData & curEmbeddedFileData = getEmbeddedFileDataByIndex(fd);
	return curEmbeddedFileData.fileSize;
}
//-----------------------------------------------------------------------
// writes data to the file
zzip_ssize_t EmbeddedZipArchiveFactory_write(int fd, _zzip_const void* buf, zzip_size_t len)
{
	// the files in this case are read only - return an error  - nonzero value.
	return -1;
}
//-----------------------------------------------------------------------
EmbeddedZipArchiveFactory::EmbeddedZipArchiveFactory()
	: TemplateObjectFactory<ZipArchive>()
{
	// init static member
	if (mPluginIo == nullptr)
	{
		mPluginIo = &sEmbeddedZipArchiveFactory_PluginIo;
		mPluginIo->fd.open = EmbeddedZipArchiveFactory_open;
		mPluginIo->fd.close = EmbeddedZipArchiveFactory_close;
		mPluginIo->fd.read = EmbeddedZipArchiveFactory_read;
		mPluginIo->fd.seeks = EmbeddedZipArchiveFactory_seeks;
		mPluginIo->fd.filesize = EmbeddedZipArchiveFactory_filesize;
		mPluginIo->fd.write = EmbeddedZipArchiveFactory_write;
		mPluginIo->fd.sys = 1;
		mPluginIo->fd.type = 1;
	}
}
//-----------------------------------------------------------------------
const String& EmbeddedZipArchiveFactory::getType() const
{
	static String name = "EmbeddedZip";
	return name;
}
//-----------------------------------------------------------------------
Object* EmbeddedZipArchiveFactory::createInstance(const String& name, const String& guid)
{
	ZipArchive* pObj = static_cast<ZipArchive*>(TemplateObjectFactory<ZipArchive>::createInstance(name, guid));
	if (pObj != nullptr)
	{
		pObj->initializeZip(mPluginIo);
	}
	return pObj;
}
//-----------------------------------------------------------------------
void EmbeddedZipArchiveFactory::addEmbbeddedFile(const String& name, const u2uint8 * fileData,
	size_t fileSize, DecryptEmbeddedZipFileFunc decryptFunc)
{
	static bool needToInit = true;
	if (needToInit)
	{
		needToInit = false;

		// we can't be sure when global variables get initialized
		// meaning it is possible our list has not been init when this
		// function is being called. The solution is to use local
		// static members in this function an init the pointers for the
		// global here. We know for use that the local static variables
		// are create in this stage.
		static FileNameToIndexMap sFileNameToIndexMap;
		static EmbbedFileDataList sEmbbedFileDataList;
		EmbeddedZipArchiveFactory_mFileNameToIndexMap = &sFileNameToIndexMap;
		EmbeddedZipArchiveFactory_mEmbbedFileDataList = &sEmbbedFileDataList;
	}

	EmbeddedFileData newEmbeddedFileData;
	newEmbeddedFileData.curPos = 0;
	newEmbeddedFileData.isFileOpened = false;
	newEmbeddedFileData.fileData = fileData;
	newEmbeddedFileData.fileSize = fileSize;
	newEmbeddedFileData.decryptFunc = decryptFunc;
	EmbeddedZipArchiveFactory_mEmbbedFileDataList->push_back(newEmbeddedFileData);
	(*EmbeddedZipArchiveFactory_mFileNameToIndexMap)[name] = static_cast<int>(EmbeddedZipArchiveFactory_mEmbbedFileDataList->size());
}
//-----------------------------------------------------------------------
void EmbeddedZipArchiveFactory::removeEmbbeddedFile(const String& name)
{
	EmbeddedZipArchiveFactory_mFileNameToIndexMap->erase(name);
}