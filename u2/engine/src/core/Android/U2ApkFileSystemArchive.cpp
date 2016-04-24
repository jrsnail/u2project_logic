#include "U2ApkFileSystemArchive.h"

#include "U2Exception.h"
#include "U2StringUtil.h"
#include "U2MemoryStream.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
static map<String, vector< String >::type >::type mFiles;
//-----------------------------------------------------------------------
bool IsFolderParsed(String Folder) 
{
	bool parsed = false;
	map<String, vector< String >::type >::iterator iter = mFiles.find(Folder);
	if (iter != mFiles.end()) parsed = true;
	return parsed;
}
//-----------------------------------------------------------------------
void ParseFolder(AAssetManager* AssetMgr, String Folder) 
{
	vector<String>::type mFilenames;
	AAssetDir* dir = AAssetManager_openDir(AssetMgr, Folder.c_str());
	const char* fileName = NULL;
	while ((fileName = AAssetDir_getNextFileName(dir)) != NULL) {
		mFilenames.push_back(String(fileName));
	}
	mFiles.insert(std::make_pair(Folder, mFilenames));
}
//-----------------------------------------------------------------------
ApkFileSystemArchive::ApkFileSystemArchive(const String& type, const String& name)
    : Archive(type, name)
	, mAssetMgr(nullptr)
{
    
}
//-----------------------------------------------------------------------
ApkFileSystemArchive::~ApkFileSystemArchive()
{
	map<String, vector< String >::type >::iterator iter = mFiles.find(mName);
	iter->second.clear();
	mFiles.erase(iter);
	unload();
}
//-----------------------------------------------------------------------
void ApkFileSystemArchive::initializeAAsset(AAssetManager* AssetMgr)
{
	mAssetMgr = AssetMgr;
}
//-----------------------------------------------------------------------
bool ApkFileSystemArchive::isCaseSensitive(void) const
{
	return true;
}
//-----------------------------------------------------------------------
void ApkFileSystemArchive::load()
{
    // nothing to do here
}
//-----------------------------------------------------------------------
void ApkFileSystemArchive::unload()
{
    // nothing to see here, move along
}
//-----------------------------------------------------------------------
InStreamPtr ApkFileSystemArchive::openForRead(const String& filename)
{
	U2_LOCK_AUTO_MUTEX;
	InStreamPtr stream;
	AAsset* asset = AAssetManager_open(mAssetMgr, (mPathPreFix + filename).c_str(), AASSET_MODE_BUFFER);
	if (asset)
	{
		off_t length = AAsset_getLength(asset);
		u2byte* membuf = static_cast<u2byte*>(U2_MALLOC(length, MEMCATEGORY_GENERAL));
		memcpy(membuf, AAsset_getBuffer(asset), length);
		AAsset_close(asset);

		stream = InStreamPtr(U2_NEW MemoryInStream(BLANK, std::shared_ptr<u2byte>(membuf, freePtr<u2byte>), length), deletePtr<MemoryInStream>);
	}
	return stream;
}
//---------------------------------------------------------------------
OutStreamPtr ApkFileSystemArchive::openForWrite(const String& filename, bool append)
{
	U2_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
		"Modification of apk archives is not supported",
		"ApkFileSystemArchive::openForWrite");
}
//---------------------------------------------------------------------
OutStreamPtr ApkFileSystemArchive::create(const String& filename)
{
	U2_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
		"Modification of apk archives is not supported",
		"ApkFileSystemArchive::create");
}
//---------------------------------------------------------------------
void ApkFileSystemArchive::remove(const String& filename)
{
	U2_EXCEPT(Exception::ERR_NOT_IMPLEMENTED,
		"Modification of apk archives is not supported",
		"ApkFileSystemArchive::remove");
}
//-----------------------------------------------------------------------
StringVectorPtr ApkFileSystemArchive::list(bool recursive, bool dirs)
{
	U2_LOCK_AUTO_MUTEX;
	StringVectorPtr files = StringVectorPtr(U2_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), deleteTPtr<StringVector>);
	map<String, vector< String >::type >::iterator iter = mFiles.find(m_szName);
	vector< String >::type fileList = iter->second;
	for (size_t i = 0; i < fileList.size(); i++)
	{
		files->push_back(fileList[i]);
	}
	return files;
}
//-----------------------------------------------------------------------
FileInfoListPtr ApkFileSystemArchive::listFileInfo(bool recursive, bool dirs)
{
	U2_LOCK_AUTO_MUTEX;
	FileInfoListPtr files(U2_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), deleteTPtr<FileInfoList>);
	map<String, vector< String >::type >::iterator iter = mFiles.find(m_szName);
	vector< String >::type fileList = iter->second;
	for (size_t i = 0; i < fileList.size(); i++)
	{
		AAsset* asset = AAssetManager_open(mAssetMgr, (mPathPreFix + fileList[i]).c_str(), AASSET_MODE_UNKNOWN);
		if (asset)
		{
			FileInfo info;
			info.archive = this;
			info.filename = fileList[i];
			info.path = m_szName;
			info.basename = fileList[i];
			info.compressedSize = AAsset_getLength(asset);
			info.uncompressedSize = info.compressedSize;
			files->push_back(info);
			AAsset_close(asset);
		}
	}
	return files;
}
//-----------------------------------------------------------------------
StringVectorPtr ApkFileSystemArchive::find(const String& pattern,
                                        bool recursive, bool dirs)
{
	U2_LOCK_AUTO_MUTEX;
	StringVectorPtr files = StringVectorPtr(U2_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), deleteTPtr<StringVector>);
	map<String, vector< String >::type >::iterator iter = mFiles.find(m_szName);
	vector< String >::type fileList = iter->second;
	for (size_t i = 0; i < fileList.size(); i++)
	{
		if (StringUtil::match(fileList[i], pattern))
			files->push_back(fileList[i]);
	}
	return files;
}
//-----------------------------------------------------------------------
FileInfoListPtr ApkFileSystemArchive::findFileInfo(const String& pattern, 
    bool recursive, bool dirs)
{
	U2_LOCK_AUTO_MUTEX;
	FileInfoListPtr files = FileInfoListPtr(U2_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), deleteTPtr<FileInfoList>);
	map<String, vector< String >::type >::iterator iter = mFiles.find(m_szName);
	vector< String >::type fileList = iter->second;
	for (size_t i = 0; i < fileList.size(); i++)
	{
		if (StringUtil::match(fileList[i], pattern))
		{
			AAsset* asset = AAssetManager_open(mAssetMgr, (mPathPreFix + fileList[i]).c_str(), AASSET_MODE_UNKNOWN);
			if (asset) 
			{
				FileInfo info;
				info.archive = this;
				info.filename = fileList[i];
				info.path = m_szName;
				info.basename = fileList[i];
				info.compressedSize = AAsset_getLength(asset);
				info.uncompressedSize = info.compressedSize;
				files->push_back(info);
				AAsset_close(asset);
			}
		}
	}
	return files;
}
//-----------------------------------------------------------------------
bool ApkFileSystemArchive::exists(const String& filename)
{
	U2_LOCK_AUTO_MUTEX;
	AAsset* asset = AAssetManager_open(mAssetMgr, (mPathPreFix + filename).c_str(), AASSET_MODE_UNKNOWN);
	if (asset)
	{
		AAsset_close(asset);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------
time_t ApkFileSystemArchive::getModifiedTime(const String& filename)
{
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
ApkFileSystemArchiveFactory::ApkFileSystemArchiveFactory(AAssetManager* AssetMgr)
	: TemplateObjectFactory<ApkFileSystemArchive>()
	, mAssetMgr(AssetMgr)
{

}
//-----------------------------------------------------------------------
Object* ApkFileSystemArchiveFactory::createInstance(const String& name)
{
	ApkFileSystemArchive* pObj = static_cast<ApkFileSystemArchive*>(TemplateObjectFactory<ApkFileSystemArchive>::createInstance(name));
	if (pObj != nullptr)
	{
		pObj->initializeAAsset(mAssetMgr);
	}
	return pObj;
}
//-----------------------------------------------------------------------
const String& ApkFileSystemArchiveFactory::getType() const
{
	static String name = "ApkFileSystem";
	return name;
}