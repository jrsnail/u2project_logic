#include "U2ApkZipArchive.h"



U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
ApkZipArchiveFactory::ApkZipArchiveFactory(AAssetManager* assetMgr)
	: TemplateObjectFactory<ZipArchive>()
	, mAssetMgr(assetMgr)
{
}
//-----------------------------------------------------------------------
const String& ApkZipArchiveFactory::getType() const
{
	static String name = "ApkZip";
	return name;
}
//-----------------------------------------------------------------------
Object* ApkZipArchiveFactory::createInstance(const String& name)
{
	String apkName = name;
	if (apkName.size() > 0 && apkName[0] == '/')
		apkName.erase(apkName.begin());

	AAsset* asset = AAssetManager_open(mAssetMgr, apkName.c_str(), AASSET_MODE_BUFFER);
	if (asset)
	{
		EmbeddedZipArchiveFactory::addEmbbeddedFile(apkName, (const u2::u2uint8*)AAsset_getBuffer(asset), AAsset_getLength(asset), 0);
	}

	return EmbeddedZipArchiveFactory::createInstance(apkName);
}