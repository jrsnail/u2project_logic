#ifndef __U2ApkZipArchive_H__
#define __U2ApkZipArchive_H__

#include "U2Prerequisites.h"
#include "U2ZipArchive.h"
#include <android/asset_manager.h>


U2EG_NAMESPACE_BEGIN


/** \addtogroup Core
*  @{
*/
/** \addtogroup Resources
*  @{
*/


class ApkZipArchiveFactory : public EmbeddedZipArchiveFactory
{
public:
	ApkZipArchiveFactory(AAssetManager* assetMgr) : mAssetMgr(assetMgr) {}

	virtual const String& getType() const override;

	virtual Object* createInstance(const String& name) override;

protected:
	AAssetManager* mAssetMgr;
};


/** @} */
/** @} */


U2EG_NAMESPACE_END


#endif // __U2ApkZipArchive_H__
